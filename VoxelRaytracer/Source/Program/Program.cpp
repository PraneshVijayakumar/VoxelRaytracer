#include "Program.h"
#include <Forge/Source/Core/Logging.h>
#include <Forge/Source/Core/Math/IntersectionTests.h>
#include "VoxelGrid.h"
#include <VoxelRaytracer/Source/Rendering/LoadingScreenRenderer/LoadingScreenRenderer.h>
#include <Forge/Source/Core/Utility.h>
#include <VoxelRaytracer/Source/Program/Editor/Editor.h>
#include <Forge/Source/Platform/Windows/FileEditors/TextFileEditor.h>
#include <Forge/Source/Core/Assert.h>

namespace forge
{
	void Program::run(HINSTANCE pHInstance, int pNCmdShow)
	{
		this->startup(pHInstance, pNCmdShow);

		this->runProgramLoop();

		this->shutdown();
	}

	void Program::getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs)
	{
		if(this->editor)
		{
			this->editor->getCurrentRenderingData(pDebugAABBs);
		}
	}

	void Program::startup(HINSTANCE pHInstance, int pNCmdShow)
	{
		this->window = new Window();
		this->window->startup(pHInstance, pNCmdShow);
		this->window->hideCursor();

		this->shouldLoadingScreenThreadExit = false;
		this->loadingScreenThread.start<Program, &Program::loadingScreenThreadEntry>(this);

		this->voxelGrid = new VoxelGrid(10);

		this->openCLManager = new OpenCLManager();
		this->openCLManager->startup();

		TextFileEditor *generateVoxelKernelTextEditor = new TextFileEditor;
		generateVoxelKernelTextEditor->openFile("ForgeData/OpenCL/Kernels/GenerateVoxel.cl");
		this->generateVoxelKernel = new WorldGeneratorKernel(this->openCLManager, generateVoxelKernelTextEditor, "generateVoxelKernel");
		delete generateVoxelKernelTextEditor;
		
		TextFileEditor *generateNodeKernelTextEditor = new TextFileEditor;
		generateNodeKernelTextEditor->openFile("ForgeData/OpenCL/Kernels/GenerateNode.cl");
		this->generateNodeKernel = new GenerateNodeKernel(this->openCLManager, generateNodeKernelTextEditor, "generateNodeKernel");
		delete generateNodeKernelTextEditor;

		// Used to store the return code from OpenCL functions
		cl_int returnCode;
		// Create and fill the voxel data buffer on the device
		this->kernelVoxelDataBuffer = clCreateBuffer(this->openCLManager->getContext(), CL_MEM_READ_ONLY, sizeof(VoxelData) * (*this->voxelGrid->getVoxelDataArray()).getNumberOfElements(), NULL, &returnCode);
		clEnqueueWriteBuffer(this->openCLManager->getCommandQueue(),
							 this->kernelVoxelDataBuffer,
							 CL_BLOCKING,
							 0,
							 sizeof(VoxelData) * this->voxelGrid->getVoxelDataArray()->getNumberOfElements(),
							 &(*this->voxelGrid->getVoxelDataArray())[0],
							 NULL,
							 NULL,
							 NULL);

		this->generateWorld();

		this->camera = new Camera(this->window->getInput());
		this->camera->setIsMouseLookEnabled(false);

		this->renderer = new OpenCLRenderer();
		this->renderer->startup(this, this->kernelVoxelDataBuffer);

		this->fpsCounter = new FPSCounter();

		this->window->getInput()->onGKeyReleased.addListener<Program, &Program::onGKeyReleased>(this);
		this->window->getInput()->onVKeyReleased.addListener<Program, &Program::onVKeyReleased>(this);
		this->window->getInput()->onMouseRightButtonDown.addListener<Program, &Program::onMouseRightButtonDown>(this);
		this->window->getInput()->onMouseRightButtonUp.addListener<Program, &Program::onMouseRightButtonUp>(this);

		this->shouldLoadingScreenThreadExitMutex.lock();
		this->shouldLoadingScreenThreadExit = true;
		this->shouldLoadingScreenThreadExitMutex.unlock();
		this->loadingScreenThread.waitForExit();

		this->window->showCursor();
		this->camera->setIsMouseLookEnabled(this->window->getInput()->getIsRightMouseButtonPressed());
	}

	void Program::loadingScreenThreadEntry()
	{
		LoadingScreenRenderer loadingScreenRenderer;
		loadingScreenRenderer.startup(this);

		Timer programLoopTimer;
		Int64 previousTimePassed = programLoopTimer.getTimePassed();
		// This is the loading screen loop
		while(true)
		{
			// Exit from the thread if it is time to do so
			{
				bool shouldLoadingScreenThreadExitLocalCopy;
				this->shouldLoadingScreenThreadExitMutex.lock();
				shouldLoadingScreenThreadExitLocalCopy = this->shouldLoadingScreenThreadExit;
				this->shouldLoadingScreenThreadExitMutex.unlock();
				if(shouldLoadingScreenThreadExitLocalCopy && programLoopTimer.getTimePassed() >= 1500000)
				{
					break;
				}
			}

			Int64 currentTimePassed = programLoopTimer.getTimePassed();
			Int64 deltaTimeInMicroseconds = currentTimePassed - previousTimePassed;
			previousTimePassed = currentTimePassed;
			Float deltaTimeInSeconds = static_cast<Float>(static_cast<Double>(deltaTimeInMicroseconds) / 1000000.0);

			// Render a new frame
			loadingScreenRenderer.render(deltaTimeInSeconds);
		}

		loadingScreenRenderer.shutdown();
	}

	void Program::generateWorld()
	{
		// Used to store the return code from OpenCL functions
		cl_int returnCode;

		// Generate the voxels
		{
			this->generateVoxelKernel->setKernelVoxelArrayBuffer(this->kernelVoxelDataBuffer);
			this->generateVoxelKernel->setVoxelOctreeDepth(this->voxelGrid->getOctreeDepth());
			this->generateVoxelKernel->setOffset(this->worldOffset);

			// Run the kernel
			size_t globalWorkSize[3];
			globalWorkSize[0] = static_cast<size_t>(this->voxelGrid->getSideLength());
			globalWorkSize[1] = static_cast<size_t>(this->voxelGrid->getSideLength());
			globalWorkSize[2] = static_cast<size_t>(this->voxelGrid->getSideLength());
			returnCode = (clEnqueueNDRangeKernel(this->openCLManager->getCommandQueue(), this->generateVoxelKernel->getKernel(), 3, NULL, globalWorkSize, NULL, 0, NULL, NULL));

			SizeType numberOfVoxels = this->voxelGrid->getSideLength() * this->voxelGrid->getSideLength() * this->voxelGrid->getSideLength();
		}
		
		// Generate all the parent nodes
		{
			this->generateNodeKernel->setKernelVoxelArrayBuffer(this->kernelVoxelDataBuffer);

			UInt32 currentGridSideLength = this->voxelGrid->getSideLength() / 2;
			for(UInt32 octreeDepth = this->voxelGrid->getOctreeDepth() - 1; octreeDepth != 0; --octreeDepth)
			{
				this->generateNodeKernel->setVoxelOctreeDepth(octreeDepth);

				// Run the kernel
				size_t globalWorkSize[3];
				globalWorkSize[0] = static_cast<size_t>(currentGridSideLength);
				globalWorkSize[1] = static_cast<size_t>(currentGridSideLength);
				globalWorkSize[2] = static_cast<size_t>(currentGridSideLength);
				returnCode = (clEnqueueNDRangeKernel(this->openCLManager->getCommandQueue(), this->generateNodeKernel->getKernel(), 3, NULL, globalWorkSize, NULL, 0, NULL, NULL));
				currentGridSideLength /= 2;
			}
		}
	}

	void Program::runProgramLoop()
	{
		// Display a frame to the user before starting to take input
		this->renderer->render();

		const Float maxFrameTime = 1.0f / 30.0f;

		Timer programLoopTimer;
		Int64 previousTimePassed = programLoopTimer.getTimePassed();

		// This is the main program loop
		while(this->window->processMessages())
		{
			const Int64 currentTimePassed = programLoopTimer.getTimePassed();
			const Int64 deltaTimeInMicroseconds = currentTimePassed - previousTimePassed;
			previousTimePassed = currentTimePassed;
			Float deltaTimeInSeconds = static_cast<Float>(static_cast<Double>(deltaTimeInMicroseconds) / 1000000.0);
			deltaTimeInSeconds = min<Float>(deltaTimeInSeconds, maxFrameTime);
			
			this->update(deltaTimeInSeconds, currentTimePassed);

			// Render a new frame
			this->renderer->render();

			this->fpsCounter->simulate(programLoopTimer.getTimePassed());
		}
	}

	void Program::update(Float pDeltaTime, Int64 pTimePassed)
	{
		if(this->worldScrollingEnabled)
		{
			this->worldOffset += pDeltaTime * 10.0f;
			this->generateWorld();
		}

		this->camera->update(pDeltaTime);

		if(this->editor)
		{
			this->editor->update();
		}
	}

	void Program::shutdown()
	{
		this->window->getInput()->onGKeyReleased.removeListener<Program, &Program::onGKeyReleased>(this);
		this->window->getInput()->onVKeyReleased.removeListener<Program, &Program::onVKeyReleased>(this);
		this->window->getInput()->onMouseRightButtonDown.removeListener<Program, &Program::onMouseRightButtonDown>(this);
		this->window->getInput()->onMouseRightButtonUp.removeListener<Program, &Program::onMouseRightButtonUp>(this);

		delete this->fpsCounter;

		this->renderer->shutdown();
		delete this->renderer;

		if(this->editor)
		{
			this->editor->shutdown();
			delete this->editor;
		}

		delete this->camera;

		FORGE_OPENCL_ERROR_CODE_CHECK(clReleaseMemObject(this->kernelVoxelDataBuffer));

		delete this->voxelGrid;

		delete this->generateVoxelKernel;
		delete this->generateNodeKernel;

		this->openCLManager->shutdown();
		delete this->openCLManager;

		this->window->shutdown();
		delete this->window;
	}

	void Program::onGKeyReleased()
	{
		this->worldScrollingEnabled = !this->worldScrollingEnabled;

		if(this->worldScrollingEnabled)
		{
			this->editor->shutdown();
			delete this->editor;
			this->editor = nullptr;
		}
		else
		{
			// Copy the voxels from OpenCL device memory to the VoxelGrid object
			FORGE_OPENCL_ERROR_CODE_CHECK(clEnqueueReadBuffer(this->openCLManager->getCommandQueue(),
										  this->kernelVoxelDataBuffer,
										  CL_BLOCKING,
										  0,
										  sizeof(VoxelData) * this->voxelGrid->getVoxelDataArray()->getNumberOfElements(),
										  &((*this->voxelGrid->getVoxelDataArray())[0]),
										  0,
										  NULL,
										  NULL));

			this->editor = new Editor;
			this->editor->startup(this);
		}
	}

	void Program::onVKeyReleased()
	{
		this->voxelOctreeVisualizationEnabled = !this->voxelOctreeVisualizationEnabled;
	}

	void Program::onMouseRightButtonDown(Int pXCoord, Int pYCoord)
	{
		this->window->hideCursor();
		this->lastCursorXCoordBeforeCameraView = pXCoord;
		this->lastCursorYCoordBeforeCameraView = pYCoord;
		this->camera->setIsMouseLookEnabled(true);
	}

	void Program::onMouseRightButtonUp(Int pXCoord, Int pYCoord)
	{
		this->camera->setIsMouseLookEnabled(false);
		this->window->setCursorCoordinates(this->lastCursorXCoordBeforeCameraView, this->lastCursorYCoordBeforeCameraView);
		this->window->showCursor();
	}
}