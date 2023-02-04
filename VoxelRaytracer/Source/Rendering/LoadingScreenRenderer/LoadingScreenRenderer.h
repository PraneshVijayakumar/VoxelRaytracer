#ifndef FORGE_LOADING_SCREEN_RENDERER_H
#define FORGE_LOADING_SCREEN_RENDERER_H

#include <Forge/Source/Platform/Windows/Window.h>
#include <VoxelRaytracer/Source/Program/VoxelGrid.h>
#include <VoxelRaytracer/Source/Program/Camera.h>
#include <Forge/Source/Platform/Windows/Timer.h>
#include <Forge/Source/Core/Containers/LinkedList.h>
#include <Forge/Source/Core/Containers/DynamicArray.h>

namespace forge
{
	class Program;

	class LoadingScreenRenderer
	{
	public:

		void startup(Program *pProgram);

		void shutdown();

		void render(Float pDeltaTime);

	private:

		const Program *program;
		Framebuffer *backbuffer = nullptr;
		Float circleCenterAngles[3];
	};
}

#endif