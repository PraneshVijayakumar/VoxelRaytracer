#include "OpenCLRenderer.h"
#include <Forge/Source/Core/Containers/DynamicArray.h>
#include <Forge/Source/Platform/OpenCL/OpenCLLogging.h>
#include <Forge/Source/Platform/OpenCL/OpenCLUtility.h>
#include <VoxelRaytracer/Source/Program/Program.h>
#include <Forge/Source/Platform/Windows/FileEditors/TextFileEditor.h>
#include <VoxelRaytracer/Source/Program/Editor/Editor.h>
#include "Kernels/DirtTextureKernel.h"

namespace forge
{
	struct OpenCLAABB
	{
		cl_float3 minPoint;
		cl_float3 maxPoint;
	};

	struct OpenCLDebugAABBData
	{
		OpenCLAABB aabb;
		cl_float3 color;
	};

	void OpenCLRenderer::startup(Program *pProgram, cl_mem pKernelVoxelDataBuffer)
	{
		this->program = pProgram;
		this->kernelVoxelDataBuffer = pKernelVoxelDataBuffer;

		// Create a backbuffer that is has the same resolution as the window
		this->backbuffer = new Framebuffer();
		this->backbuffer->startup(pProgram->getWindow()->getResolutionWidth(), pProgram->getWindow()->getResolutionHeight());

		// Used to store the return code from OpenCL functions
		cl_int returnCode;

		this->kernelTexturesBuffer = clCreateBuffer(this->program->getOpenCLManager()->getContext(), CL_MEM_READ_ONLY, sizeof(Float) * this->textureSize * this->textureSize * 255, NULL, &returnCode);
		FORGE_OPENCL_ERROR_CODE_CHECK(returnCode);
		this->generateTextures();
		this->kernelUInt8FrameBuffer = clCreateBuffer(this->program->getOpenCLManager()->getContext(), CL_MEM_WRITE_ONLY, this->backbuffer->getPixelsMemorySize(), NULL, &returnCode);
		FORGE_OPENCL_ERROR_CODE_CHECK(returnCode);
		this->kernelDebugAABBsBuffer = clCreateBuffer(this->program->getOpenCLManager()->getContext(), CL_MEM_READ_ONLY, sizeof(OpenCLDebugAABBData) * this->maxNumberOfDebugAABBs, NULL, &returnCode);
		FORGE_OPENCL_ERROR_CODE_CHECK(returnCode);

		TextFileEditor renderPixelKernelTextEditor;
		renderPixelKernelTextEditor.openFile("ForgeData/OpenCL/Kernels/Renderer.cl");
		this->renderPixelKernel = new RenderPixelKernel(this->program->getOpenCLManager(),
														&renderPixelKernelTextEditor,
														"renderPixel");
		this->renderPixelKernel->setKernelVoxelArrayBuffer(this->kernelVoxelDataBuffer);
		this->renderPixelKernel->setOutputUInt8Framebuffer(this->kernelUInt8FrameBuffer);
		this->renderPixelKernel->setKernelTexturesBuffer(this->kernelTexturesBuffer);
		this->renderPixelKernel->setKernelDebugAABBsBuffer(this->kernelDebugAABBsBuffer);

		this->program->getVoxelGrid()->onVoxelOctreeNodeUpdated.addListener<OpenCLRenderer, &OpenCLRenderer::onVoxelNodeUpdated>(this);
	}

	void OpenCLRenderer::shutdown()
	{
		this->program->getVoxelGrid()->onVoxelOctreeNodeUpdated.removeListener<OpenCLRenderer, &OpenCLRenderer::onVoxelNodeUpdated>(this);

		// Clean up OpenCL resources
		FORGE_OPENCL_ERROR_CODE_CHECK(clReleaseMemObject(this->kernelUInt8FrameBuffer));
		FORGE_OPENCL_ERROR_CODE_CHECK(clReleaseMemObject(this->kernelTexturesBuffer));

		delete this->renderPixelKernel;

		this->backbuffer->shutdown();
		delete this->backbuffer;
	}

	void OpenCLRenderer::render()
	{
		// Pass arguments to the parameters of the renderer kernel
		{
			this->renderPixelKernel->setCameraPosition(this->program->getCamera()->coordinateSpace.position);
			this->renderPixelKernel->setCameraOrientationXAxis(this->program->getCamera()->coordinateSpace.orientation.getXAxis());
			this->renderPixelKernel->setCameraOrientationYAxis(this->program->getCamera()->coordinateSpace.orientation.getYAxis());
			this->renderPixelKernel->setCameraOrientationZAxis(this->program->getCamera()->coordinateSpace.orientation.getZAxis());
			this->renderPixelKernel->setCameraVerticalFieldOfViewInDegrees(this->program->getCamera()->getVerticalFieldOfViewInDegrees());
			this->renderPixelKernel->setVoxelOctreeDepth(this->program->getVoxelGrid()->getOctreeDepth());
			this->renderPixelKernel->setIsOctreeVisualizationEnabled(this->program->getIsVoxelOctreeVisualizationEnabled());
		}

		// Update the voxel octree data in the device memory if needed
		{
			if(this->shouldUpdateVoxelOctreeBuffer)
			{
				clEnqueueWriteBuffer(this->program->getOpenCLManager()->getCommandQueue(),
									 this->kernelVoxelDataBuffer,
									 CL_BLOCKING,
									 0,
									 sizeof(VoxelData) * this->program->getVoxelGrid()->getVoxelDataArray()->getNumberOfElements(),
									 &(*this->program->getVoxelGrid()->getVoxelDataArray())[0],
									 NULL,
									 NULL,
									 NULL);
				
				this->shouldUpdateVoxelOctreeBuffer = false;
			}
		}
		
		// Update the debug AABBs data in the device memory
		{
			DynamicArray<DebugAABBData> debugAABBs;
			this->program->getCurrentRenderingData(debugAABBs);
			Int32 numberOfDebugAABBsInThisFrame = static_cast<Int32>(min<SizeType>(debugAABBs.getNumberOfElements(), this->maxNumberOfDebugAABBs));
			ResizableArray<OpenCLDebugAABBData> openCLDebugAABBs(debugAABBs.getNumberOfElements());
			for(cl_int i = 0; i < numberOfDebugAABBsInThisFrame; ++i)
			{
				openCLDebugAABBs[i].aabb.minPoint = convertVector3Tocl_float3(debugAABBs[i].aabb.getMinPoint());
				openCLDebugAABBs[i].aabb.maxPoint = convertVector3Tocl_float3(debugAABBs[i].aabb.getMaxPoint());
				openCLDebugAABBs[i].color = convertVector3Tocl_float3(debugAABBs[i].color.getRGBVector3());
			}
			clEnqueueWriteBuffer(this->program->getOpenCLManager()->getCommandQueue(), this->kernelDebugAABBsBuffer, CL_BLOCKING, 0, sizeof(OpenCLDebugAABBData) * numberOfDebugAABBsInThisFrame, &openCLDebugAABBs[0], NULL, NULL, NULL);

			// Set the number of debug AABBs
			this->renderPixelKernel->setNumberOfDebugAABBs(numberOfDebugAABBsInThisFrame);
		}

		// Run the kernel
		{
			size_t globalWorkSize[2];
			globalWorkSize[0] = static_cast<size_t>(this->backbuffer->getWidth());
			globalWorkSize[1] = static_cast<size_t>(this->backbuffer->getHeight());
			FORGE_OPENCL_ERROR_CODE_CHECK(clEnqueueNDRangeKernel(this->program->getOpenCLManager()->getCommandQueue(), this->renderPixelKernel->getKernel(), 2, NULL, globalWorkSize, NULL, 0, NULL, NULL));
		}

		// Copy the pixels from device memory to the backbuffer
		FORGE_OPENCL_ERROR_CODE_CHECK(clEnqueueReadBuffer(this->program->getOpenCLManager()->getCommandQueue(), this->kernelUInt8FrameBuffer, CL_BLOCKING, 0, this->backbuffer->getPixelsMemorySize(), this->backbuffer->getPixelsMemoryPointer(), 0, NULL, NULL));

		// Render the framerate text
		{
			char framesPerSecondText[256] = "0";
			Int numberOfCharactersInString = snprintf(framesPerSecondText, sizeof(framesPerSecondText), "FPS : %lld", this->program->getFPSCounter()->getCurrentFPS());
			this->backbuffer->tempDrawText(0, 0, framesPerSecondText, numberOfCharactersInString);
		}

		this->program->getWindow()->blitFramebufferToScreen(*this->backbuffer);
	}

	void OpenCLRenderer::generateTextures()
	{
		TextFileEditor dirtTextureTextEditor;
		dirtTextureTextEditor.openFile("ForgeData/OpenCL/Kernels/DirtTexture.cl");

		DirtTextureKernel dirtTextureKernel(this->program->getOpenCLManager(), &dirtTextureTextEditor, "dirtTextureKernel");
		dirtTextureKernel.setTextureBuffer(this->kernelTexturesBuffer);
		dirtTextureKernel.setTextureIndex(0);

		// Run the kernel
		size_t globalWorkSize[2];
		globalWorkSize[0] = static_cast<size_t>(this->textureSize);
		globalWorkSize[1] = static_cast<size_t>(this->textureSize);
		FORGE_OPENCL_ERROR_CODE_CHECK(clEnqueueNDRangeKernel(this->program->getOpenCLManager()->getCommandQueue(), dirtTextureKernel.getKernel(), 2, NULL, globalWorkSize, NULL, 0, NULL, NULL));
	}

	void OpenCLRenderer::onVoxelNodeUpdated(UInt32 pMortonCode, UInt8 pHasChildBitfield)
	{
		this->shouldUpdateVoxelOctreeBuffer = true;
	}
}