#ifndef FORGE_WORLD_GENERATOR_KERNEL_H
#define FORGE_WORLD_GENERATOR_KERNEL_H

#include <Forge/Source/Platform/OpenCL/OpenCLKernel.h>

namespace forge
{
	class WorldGeneratorKernel : public OpenCLKernel
	{
	public:

		WorldGeneratorKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName);

		void setKernelVoxelArrayBuffer(cl_mem pKernelVoxelArrayBuffer);

		void setVoxelOctreeDepth(UInt32 pVoxelOctreeDepth);

		void setOffset(Float pOffset);
	};

	inline
	WorldGeneratorKernel::WorldGeneratorKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName) :
	OpenCLKernel(pOpenCLManager, pTextFileEditor, pKernelName)
	{
	}

	inline
	void WorldGeneratorKernel::setKernelVoxelArrayBuffer(cl_mem pKernelVoxelArrayBuffer)
	{
		this->setBuffer(0, pKernelVoxelArrayBuffer);
	}

	inline
	void WorldGeneratorKernel::setVoxelOctreeDepth(UInt32 pVoxelOctreeDepth)
	{
		this->setUInt(1, pVoxelOctreeDepth);
	}

	inline
	void WorldGeneratorKernel::setOffset(Float pOffset)
	{
		this->setFloat(2, pOffset);
	}
}

#endif