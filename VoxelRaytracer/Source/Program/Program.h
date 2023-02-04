#ifndef FORGE_PROGRAM_H
#define FORGE_PROGRAM_H

#include <Forge/Source/Core/Containers/DynamicArray.h>
#include <Forge/Source/Platform/Windows/Window.h>
#include <VoxelRaytracer/Source/Rendering/OpenCLRenderer/OpenCLRenderer.h>
#include "VoxelGrid.h"
#include "Camera.h"
#include <Forge/Source/Platform/OpenCL/OpenCLKernel.h>
#include <Forge/Source/Platform/OpenCL/OpenCLManager.h>
#include <Forge/Source/Platform/Windows/Threading/Thread.h>
#include <Forge/Source/Platform/Windows/Threading/Mutex.h>
#include <VoxelRaytracer/Source/Program/WorldGenerator/Kernels/WorldGeneratorKernel.h>
#include <VoxelRaytracer/Source/Program/WorldGenerator/Kernels/GenerateNodeKernel.h>
#include <VoxelRaytracer/Source/Rendering/DebugAABBData.h>
#include "FPSCounter.h"

namespace forge
{
	class Editor;

	// The main program class
	class Program
	{
	public:

		struct VoxelUnderMouseCursorData
		{
			IntegerVector3<VoxelGrid::VoxelCoordType> coordsOfVoxelUnderMouseCursor;
			Vector3 normalOfVoxelFaceUnderMouseCursor;
		};

		void run(HINSTANCE pHInstance, int pNCmdShow);

		bool getIsMouseLookEnabled() const;

		bool getIsVoxelOctreeVisualizationEnabled() const;

		Camera* getCamera() const;

		VoxelGrid* getVoxelGrid() const;

		Window* getWindow() const;

		OpenCLManager *getOpenCLManager() const;

		const Editor* getEditor() const;

		const FPSCounter* getFPSCounter() const;

		void getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs);

	private:

		void startup(HINSTANCE pHInstance, int pNCmdShow);

		void loadingScreenThreadEntry();

		void generateWorld();

		void runProgramLoop();

		void update(Float pDeltaTime, Int64 pTimePassed);

		void shutdown();

		void onGKeyReleased();
		void onVKeyReleased();
		void onMouseRightButtonDown(Int pXCoord, Int pYCoord);
		void onMouseRightButtonUp(Int pXCoord, Int pYCoord);

		Window *window;
		OpenCLManager *openCLManager;
		VoxelGrid *voxelGrid;
		Camera *camera;
		OpenCLRenderer *renderer;
		FPSCounter *fpsCounter;

		WorldGeneratorKernel *generateVoxelKernel;
		GenerateNodeKernel *generateNodeKernel;
		cl_mem kernelVoxelDataBuffer;

		Thread loadingScreenThread;
		bool shouldLoadingScreenThreadExit = false;
		Mutex shouldLoadingScreenThreadExitMutex;

		Int lastCursorXCoordBeforeCameraView;
		Int lastCursorYCoordBeforeCameraView;

		Editor *editor = nullptr;
		Float worldOffset = 0.0f;
		bool worldScrollingEnabled = true;
		bool voxelOctreeVisualizationEnabled = true;
	};

	inline
	bool Program::getIsMouseLookEnabled() const
	{
		return this->camera->getIsMouseLookEnabled();
	}

	inline
	bool Program::getIsVoxelOctreeVisualizationEnabled() const
	{
		return this->voxelOctreeVisualizationEnabled;
	}

	inline
	Camera* Program::getCamera() const
	{
		return this->camera;
	}

	inline
	VoxelGrid* Program::getVoxelGrid() const
	{
		return this->voxelGrid;
	}

	inline
	Window* Program::getWindow() const
	{
		return this->window;
	}

	inline
	OpenCLManager *Program::getOpenCLManager() const
	{
		return this->openCLManager;
	}

	inline
	const Editor* Program::getEditor() const
	{
		return this->editor;
	}

	inline
	const FPSCounter* Program::getFPSCounter() const
	{
		return this->fpsCounter;
	}
}

#endif