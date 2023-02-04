#include "Editor.h"
#include <VoxelRaytracer/Source/Program/Program.h>

namespace forge
{
	void Editor::startup(Program *pProgram)
	{
		this->program = pProgram;

		this->editorCommandsManager = new CommandsManager();
		this->editorCommandsManager->startup();

		// Create all the editor tools
		this->rectangleEditTool = new RectangleEditTool(this);
		this->blockEditTool = new BlockEditTool(this);
		// No editor tool is selected on editor startup
		this->currentEditorTool = nullptr;

		this->currentBrushVoxelData.voxelID = 1;

		pProgram->getWindow()->getInput()->onZKeyPressed.addListener<Editor, &Editor::onZKeyPressed>(this);
		pProgram->getWindow()->getInput()->onRKeyPressed.addListener<Editor, &Editor::onRKeyPressed>(this);
		pProgram->getWindow()->getInput()->onGKeyPressed.addListener<Editor, &Editor::onGKeyPressed>(this);
		pProgram->getWindow()->getInput()->onBKeyPressed.addListener<Editor, &Editor::onBKeyPressed>(this);
		pProgram->getWindow()->getInput()->on1KeyReleased.addListener<Editor, &Editor::on1KeyReleased>(this);
		pProgram->getWindow()->getInput()->on2KeyReleased.addListener<Editor, &Editor::on2KeyReleased>(this);
		pProgram->getWindow()->getInput()->onMouseLeftButtonUp.addListener<Editor, &Editor::onMouseLeftButtonUp>(this);
	}

	void Editor::shutdown()
	{
		this->program->getWindow()->getInput()->onZKeyPressed.removeListener<Editor, &Editor::onZKeyPressed>(this);
		this->program->getWindow()->getInput()->onRKeyPressed.removeListener<Editor, &Editor::onRKeyPressed>(this);
		this->program->getWindow()->getInput()->onGKeyPressed.removeListener<Editor, &Editor::onGKeyPressed>(this);
		this->program->getWindow()->getInput()->onBKeyPressed.removeListener<Editor, &Editor::onBKeyPressed>(this);
		this->program->getWindow()->getInput()->on1KeyReleased.removeListener<Editor, &Editor::on1KeyReleased>(this);
		this->program->getWindow()->getInput()->on2KeyReleased.removeListener<Editor, &Editor::on2KeyReleased>(this);
		this->program->getWindow()->getInput()->onMouseLeftButtonUp.removeListener<Editor, &Editor::onMouseLeftButtonUp>(this);

		if(this->currentEditorTool)
		{
			this->currentEditorTool->deactivate();
			delete this->currentEditorTool;
		}

		this->editorCommandsManager->shutdown();
		delete this->editorCommandsManager;
	}

	void Editor::update()
	{
		// If mouse look is not enabled, then a voxel face is in focus
		if(!this->program->getCamera()->getIsMouseLookEnabled())
		{
			if(this->currentEditorTool)
			{
				this->currentEditorTool->update();
			}
		}
	}

	void Editor::getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs)
	{
		if(this->currentEditorTool && this->currentEditorTool->getIsActive())
		{
			this->currentEditorTool->getCurrentRenderingData(pDebugAABBs);
		}
	}

	void Editor::onRKeyPressed()
	{
		if(this->getWindow()->getInput()->getIsControlKeyPressed())
		{
			if(this->getWindow()->getInput()->getIsShiftKeyPressed())
			{
				//this->currentBrushVoxelColor.setRedUint8(this->currentBrushVoxelColor.getRedUint8() - 1);
			}
			else
			{
				//this->currentBrushVoxelColor.setRedUint8(this->currentBrushVoxelColor.getRedUint8() + 1);
			}
		}
		else
		{
			return;
		}
	}

	void Editor::onGKeyPressed()
	{
		if(this->getWindow()->getInput()->getIsControlKeyPressed())
		{
			if(this->getWindow()->getInput()->getIsShiftKeyPressed())
			{
				//this->currentBrushVoxelColor.setGreenUint8(this->currentBrushVoxelColor.getGreenUint8() - 1);
			}
			else
			{
				//this->currentBrushVoxelColor.setGreenUint8(this->currentBrushVoxelColor.getGreenUint8() + 1);
			}
		}
		else
		{
			return;
		}
	}

	void Editor::onBKeyPressed()
	{
		if(this->getWindow()->getInput()->getIsControlKeyPressed())
		{
			if(this->getWindow()->getInput()->getIsShiftKeyPressed())
			{
				//this->currentBrushVoxelColor.setBlueUint8(this->currentBrushVoxelColor.getBlueUint8() - 1);
			}
			else
			{
				//this->currentBrushVoxelColor.setBlueUint8(this->currentBrushVoxelColor.getBlueUint8() + 1);
			}
		}
		else
		{
			return;
		}
	}

	void Editor::onZKeyPressed()
	{
		// Handle editor undo and redo
		if(this->program->getWindow()->getInput()->getIsControlKeyPressed())
		{
			if(this->program->getWindow()->getInput()->getIsShiftKeyPressed())
			{
				this->editorCommandsManager->redo();
			}
			else
			{
				this->editorCommandsManager->undo();
			}
		}
	}

	void Editor::on1KeyReleased()
	{
		this->toggleEditorTool(this->rectangleEditTool);
	}

	void Editor::on2KeyReleased()
	{
		this->toggleEditorTool(this->blockEditTool);
	}

	void Editor::onMouseLeftButtonUp(Int pXCoord, Int pYCoord)
	{
	}
}