#ifndef FORGE_DIRT_TEXTURE_KERNEL_H
#define FORGE_DIRT_TEXTURE_KERNEL_H

#include <Forge/Source/Platform/OpenCL/OpenCLKernel.h>

namespace forge
{
	class DirtTextureKernel : public OpenCLKernel
	{
	public:

		DirtTextureKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName);

		void setTextureBuffer(cl_mem pTextureBuffer);

		void setTextureIndex(UInt32 pTextureIndex);
	};

	inline
	DirtTextureKernel::DirtTextureKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName) :
	OpenCLKernel(pOpenCLManager, pTextFileEditor, pKernelName)
	{
	}

	inline
	void DirtTextureKernel::setTextureBuffer(cl_mem pTextureBuffer)
	{
		this->setBuffer(0, pTextureBuffer);
	}

	inline
	void DirtTextureKernel::setTextureIndex(UInt32 pTextureIndex)
	{
		this->setUInt(1, pTextureIndex);
	}
}

#endif