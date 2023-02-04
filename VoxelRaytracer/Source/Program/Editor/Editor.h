#ifndef FORGE_EDITOR_H
#define FORGE_EDITOR_H

#include <Forge/Source/Core/Commands/CommandsManager.h>
#include <VoxelRaytracer/Source/Program/Editor/EditorTools/EditorTool.h>
#include <VoxelRaytracer/Source/Program/Program.h>
#include <VoxelRaytracer/Source/Program/Editor/EditorTools/BlockEditTool.h>
#include <VoxelRaytracer/Source/Program/Editor/EditorTools/RectangleEditTool.h>
#include <VoxelRaytracer/Source/Rendering/DebugAABBData.h>

namespace forge
{
	class Program;

	class Editor
	{
	public:

		void startup(Program *pProgram);

		void shutdown();

		void update();

		CommandsManager* getEditorCommandsManager() const;

		EditorTool* getCurrentEditorTool() const;

		Camera* getCamera() const;

		VoxelGrid* getVoxelGrid() const;

		Window* getWindow() const;

		VoxelData getCurrentBrushVoxelData() const;

		void getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs);

	private:

		void onRKeyPressed();
		void onGKeyPressed();
		void onBKeyPressed();
		void onZKeyPressed();
		void on1KeyReleased();
		void on2KeyReleased();
		void onMouseLeftButtonUp(Int pXCoord, Int pYCoord);

		template <typename EditorToolType>
		void toggleEditorTool(EditorToolType *pEditorTool);

		Program *program;

		CommandsManager *editorCommandsManager = nullptr;
		EditorTool *currentEditorTool = nullptr;
		RectangleEditTool *rectangleEditTool = nullptr;
		BlockEditTool *blockEditTool = nullptr;

		VoxelData currentBrushVoxelData;
	};

	inline
	CommandsManager* Editor::getEditorCommandsManager() const
	{
		return this->editorCommandsManager;
	}

	inline
	EditorTool* Editor::getCurrentEditorTool() const
	{
		return this->currentEditorTool;
	}

	inline
	Camera* Editor::getCamera() const
	{
		return this->program->getCamera();
	}

	inline
	VoxelGrid* Editor::getVoxelGrid() const
	{
		return this->program->getVoxelGrid();
	}

	inline
	Window* Editor::getWindow() const
	{
		return this->program->getWindow();
	}

	inline
	VoxelData Editor::getCurrentBrushVoxelData() const
	{
		return currentBrushVoxelData;
	}

	template<typename EditorToolType>
	void Editor::toggleEditorTool(EditorToolType *pEditorTool)
	{
		// If an editor tool is currently selected
		if(this->currentEditorTool)
		{
			// If the selected editor tool is the same as the one being toggled
			if(typeid(*this->currentEditorTool) == typeid(EditorToolType))
			{
				// Deselect the curent editor tool
				this->currentEditorTool->deactivate();
				this->currentEditorTool = nullptr;
			}
			else
			{
				// Deactivate the current editor tool
				this->currentEditorTool->deactivate();

				// Select the editor tool
				this->currentEditorTool = pEditorTool;
				this->currentEditorTool->activate();
			}
		}
		else
		{
			// Select the editor tool
			this->currentEditorTool = pEditorTool;
			this->currentEditorTool->activate();
		}
	}
}

#endif