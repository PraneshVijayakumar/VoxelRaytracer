#ifndef FORGE_OPENCL_RENDERER_H
#define FORGE_OPENCL_RENDERER_H

#include <CL/cl.h>
#include <Forge/Source/Platform/Windows/Framebuffer.h>
#include <Forge/Source/Platform/Windows/Window.h>
#include <Forge/Source/Platform/Windows/Timer.h>
#include <Forge/Source/Platform/OpenCL/OpenCLManager.h>
#include <Forge/Source/Core/Math/IntegerVector3.h>
#include "Kernels/RenderPixelKernel.h"
#include <Forge/Source/Platform/Windows/Threading/Thread.h>
#include <Forge/Source/Platform/Windows/Threading/Mutex.h>
#include <Forge/Source/Platform/Windows/Threading/Signal.h>
#include <Forge/Source/Core/Math/AABB.h>

namespace forge
{
	class Program;

	class OpenCLRenderer
	{
	public:

		void startup(Program *pProgram, cl_mem pKernelVoxelDataBuffer);

		void shutdown();

		void render();

	private:

		void generateTextures();

		void onVoxelNodeUpdated(UInt32 pMortonCode, UInt8 pHasChildBitfield);

		RenderPixelKernel *renderPixelKernel;

		cl_mem kernelVoxelDataBuffer;
		SizeType textureSize = 16;
		cl_mem kernelTexturesBuffer;
		cl_mem kernelUInt8FrameBuffer;
		bool shouldUpdateVoxelOctreeBuffer = false;

		cl_mem kernelDebugAABBsBuffer;
		const SizeType maxNumberOfDebugAABBs = 2;

		Program *program;
		Framebuffer *backbuffer = nullptr;
		SizeType numberOfBackbufferPixels;
	};
}

#endif