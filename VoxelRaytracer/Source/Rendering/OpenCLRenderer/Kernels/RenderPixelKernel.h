#ifndef FORGE_RENDER_PIXEL_KERNEL_H
#define FORGE_RENDER_PIXEL_KERNEL_H

#include <Forge/Source/Platform/OpenCL/OpenCLKernel.h>

namespace forge
{
	class RenderPixelKernel : public OpenCLKernel
	{
	public:

		RenderPixelKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName);

		void setCameraPosition(const Vector3 &pCameraPosition);

		void setCameraOrientationXAxis(const Vector3 &pCameraOrientationXAxis);

		void setCameraOrientationYAxis(const Vector3 &pCameraOrientationYAxis);

		void setCameraOrientationZAxis(const Vector3 &pCameraOrientationZAxis);

		void setCameraVerticalFieldOfViewInDegrees(Float pCameraVerticalFieldOfViewInDegrees);

		void setKernelVoxelArrayBuffer(cl_mem pKernelVoxelArrayBuffer);

		void setVoxelOctreeDepth(UInt32 pVoxelOctreeDepth);

		void setKernelTexturesBuffer(cl_mem pKernelTexturesBuffer);

		void setKernelDebugAABBsBuffer(cl_mem pKernelDebugAABBsBuffer);

		void setNumberOfDebugAABBs(Int32 pNumberOfDebugAABBs);

		void setIsOctreeVisualizationEnabled(bool pIsOctreeVisualizationEnabled);

		void setOutputUInt8Framebuffer(cl_mem pOutputUInt8Framebuffer);
	};

	inline
	RenderPixelKernel::RenderPixelKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName) :
	OpenCLKernel(pOpenCLManager, pTextFileEditor, pKernelName)
	{
	}

	inline
	void RenderPixelKernel::setCameraPosition(const Vector3 &pCameraPosition)
	{
		this->setFloat3(0, pCameraPosition);
	}

	inline
	void RenderPixelKernel::setCameraOrientationXAxis(const Vector3 &pCameraOrientationXAxis)
	{
		this->setFloat3(1, pCameraOrientationXAxis);
	}

	inline
	void RenderPixelKernel::setCameraOrientationYAxis(const Vector3 &pCameraOrientationYAxis)
	{
		this->setFloat3(2, pCameraOrientationYAxis);
	}

	inline
	void RenderPixelKernel::setCameraOrientationZAxis(const Vector3 &pCameraOrientationZAxis)
	{
		this->setFloat3(3, pCameraOrientationZAxis);
	}

	inline
	void RenderPixelKernel::setCameraVerticalFieldOfViewInDegrees(Float pCameraVerticalFieldOfViewInDegrees)
	{
		this->setFloat(4, pCameraVerticalFieldOfViewInDegrees);
	}

	inline
	void RenderPixelKernel::setKernelVoxelArrayBuffer(cl_mem pKernelVoxelArrayBuffer)
	{
		this->setBuffer(5, pKernelVoxelArrayBuffer);
	}

	inline
	void RenderPixelKernel::setVoxelOctreeDepth(UInt32 pVoxelOctreeDepth)
	{
		this->setUInt(6, pVoxelOctreeDepth);
	}

	inline
	void RenderPixelKernel::setKernelTexturesBuffer(cl_mem pKernelTexturesBuffer)
	{
		this->setBuffer(7, pKernelTexturesBuffer);
	}

	inline
	void RenderPixelKernel::setKernelDebugAABBsBuffer(cl_mem pDebugAABBsBuffer)
	{
		this->setBuffer(8, pDebugAABBsBuffer);
	}

	inline
	void RenderPixelKernel::setNumberOfDebugAABBs(Int32 pNumberOfDebugAABBs)
	{
		this->setInt(9, pNumberOfDebugAABBs);
	}

	inline
	void RenderPixelKernel::setIsOctreeVisualizationEnabled(bool pIsOctreeVisualizationEnabled)
	{
		if(pIsOctreeVisualizationEnabled)
		{
			this->setInt(10, 1);
		}
		else
		{
			this->setInt(10, 0);
		}
	}

	inline
	void RenderPixelKernel::setOutputUInt8Framebuffer(cl_mem pOutputUInt8Framebuffer)
	{
		this->setBuffer(11, pOutputUInt8Framebuffer);
	}
}

#endif