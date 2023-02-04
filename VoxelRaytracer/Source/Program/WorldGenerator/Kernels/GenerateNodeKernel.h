#ifndef FORGE_GENERATE_NODE_KERNEL_H
#define FORGE_GENERATE_NODE_KERNEL_H

#include <Forge/Source/Platform/OpenCL/OpenCLKernel.h>

namespace forge
{
	class GenerateNodeKernel : public OpenCLKernel
	{
	public:

		GenerateNodeKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName);

		void setKernelVoxelArrayBuffer(cl_mem pKernelVoxelArrayBuffer);

		void setVoxelOctreeDepth(UInt32 pVoxelOctreeDepth);
	};

	inline
	GenerateNodeKernel::GenerateNodeKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName) :
	OpenCLKernel(pOpenCLManager, pTextFileEditor, pKernelName)
	{
	}

	inline
	void GenerateNodeKernel::setKernelVoxelArrayBuffer(cl_mem pKernelVoxelArrayBuffer)
	{
		this->setBuffer(0, pKernelVoxelArrayBuffer);
	}

	inline
	void GenerateNodeKernel::setVoxelOctreeDepth(UInt32 pVoxelOctreeDepth)
	{
		this->setUInt(1, pVoxelOctreeDepth);
	}
}

#endif