#include "BlockEditTool.h"
#include <Forge/Source/Platform/Windows/Input.h>
#include <Forge/Source/Core/Utility.h>
#include <Forge/Source/Core/Math/MathUtility.h>
#include <VoxelRaytracer/Source/Program/Program.h>
#include <VoxelRaytracer/Source/Program/Editor/EditorCommands/SetVoxelsEditorCommand.h>
#include <VoxelRaytracer/Source/Program/ProgramIntersectionTests.h>
#include <VoxelRaytracer/Source/Program/Editor/Editor.h>

namespace forge
{
	void BlockEditTool::update()
	{
		// If mouse look is enabled, then the current focus voxel should not be updated
		if(this->getEditor()->getCamera()->getIsMouseLookEnabled())
		{
			return;
		}

		// Get the camera ray for the pixel that the mouse cursor is currently over
		Ray singleVoxelEditRay = this->getEditor()->getCamera()->getCameraRayForPixel(this->getEditor()->getWindow()->getInput()->getCurrentCursorXCoord(),
																					  this->getEditor()->getWindow()->getInput()->getCurrentCursorYCoord(),
																					  this->getEditor()->getWindow()->getResolutionWidth(),
																					  this->getEditor()->getWindow()->getResolutionHeight());

		// Find the voxel that is the current focus distance away from the camera in the direction of the camera ray
		RayAndVoxelDataIntersectionData rayAndVoxelDataIntersectionData;
		rayAndVoxelGridIntersectionTest(singleVoxelEditRay, *this->getEditor()->getVoxelGrid(), rayAndVoxelDataIntersectionData, false, this->currentFocusVoxelDistance);

		// Update the current focus voxel
		this->currentFocusVoxelCoords = rayAndVoxelDataIntersectionData.voxelCoords;
	}

	void BlockEditTool::getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs)
	{
		// If the first corner voxel has been selected, it is always displayed
		if(this->firstCornerVoxelSelected)
		{
			DebugAABBData currentFirstVoxelDebugAABBData;
			currentFirstVoxelDebugAABBData.aabb.setMinPoint(Vector3(this->firstCornerVoxelCoords.x - 0.5f, this->firstCornerVoxelCoords.y - 0.5f, this->firstCornerVoxelCoords.z - 0.5f));
			currentFirstVoxelDebugAABBData.aabb.setMaxPoint(Vector3(this->firstCornerVoxelCoords.x + 0.5f, this->firstCornerVoxelCoords.y + 0.5f, this->firstCornerVoxelCoords.z + 0.5f));
			currentFirstVoxelDebugAABBData.color.setRGBUint8(0, 255, 0);
			pDebugAABBs.append(currentFirstVoxelDebugAABBData);
		}

		// If mouse look is enabled, then the current focus voxel should not be displayed
		if(this->getEditor()->getCamera()->getIsMouseLookEnabled())
		{
			return;
		}

		// Display the current focus voxel
		DebugAABBData currentFocusVoxelDebugAABBData;
		currentFocusVoxelDebugAABBData.aabb.setMinPoint(Vector3(this->currentFocusVoxelCoords.x - 0.5f, this->currentFocusVoxelCoords.y - 0.5f, this->currentFocusVoxelCoords.z - 0.5f));
		currentFocusVoxelDebugAABBData.aabb.setMaxPoint(Vector3(this->currentFocusVoxelCoords.x + 0.5f, this->currentFocusVoxelCoords.y + 0.5f, this->currentFocusVoxelCoords.z + 0.5f));
		currentFocusVoxelDebugAABBData.color.setRGBUint8(255, 0, 0);
		pDebugAABBs.append(currentFocusVoxelDebugAABBData);
	}

	void BlockEditTool::activateImplementation()
	{
		this->getEditor()->getWindow()->getInput()->onMouseLeftButtonUp.addListener<BlockEditTool, &BlockEditTool::onMouseLeftButtonUp>(this);
		this->getEditor()->getWindow()->getInput()->onMouseScrollWheelRotated.addListener<BlockEditTool, &BlockEditTool::onMouseScrollWheelRotated>(this);

		// Reset the voxel edit distance every time the tool is activated
		this->currentFocusVoxelDistance = 5;
		this->firstCornerVoxelSelected = false;
	}

	void BlockEditTool::deactivateImplementation()
	{
		this->getEditor()->getWindow()->getInput()->onMouseLeftButtonUp.removeListener<BlockEditTool, &BlockEditTool::onMouseLeftButtonUp>(this);
		this->getEditor()->getWindow()->getInput()->onMouseScrollWheelRotated.removeListener<BlockEditTool, &BlockEditTool::onMouseScrollWheelRotated>(this);
	}

	void BlockEditTool::onMouseLeftButtonUp(Int pXCoord, Int pYCoord)
	{
		// If mouse look is enabled, then the focus voxel distance should not be edited
		if(this->getEditor()->getCamera()->getIsMouseLookEnabled())
		{
			return;
		}

		// If the first block edit voxel has already been selected
		if(this->firstCornerVoxelSelected)
		{
			// Set the second corner voxel coords to be equal to the current focus voxel coords
			this->secondCornerVoxelCoords = this->currentFocusVoxelCoords;

			// Calculate the integer coordinates of the minimum and maximum corner voxels of the block
			IntegerVector3<VoxelGrid::VoxelCoordType> selectedBlockAABBMinCoords;
			selectedBlockAABBMinCoords.x = min(this->firstCornerVoxelCoords.x, this->secondCornerVoxelCoords.x);
			selectedBlockAABBMinCoords.y = min(this->firstCornerVoxelCoords.y, this->secondCornerVoxelCoords.y);
			selectedBlockAABBMinCoords.z = min(this->firstCornerVoxelCoords.z, this->secondCornerVoxelCoords.z);
			IntegerVector3<VoxelGrid::VoxelCoordType> selectedBlockAABBMaxCoords;
			selectedBlockAABBMaxCoords.x = max(this->firstCornerVoxelCoords.x, this->secondCornerVoxelCoords.x);
			selectedBlockAABBMaxCoords.y = max(this->firstCornerVoxelCoords.y, this->secondCornerVoxelCoords.y);
			selectedBlockAABBMaxCoords.z = max(this->firstCornerVoxelCoords.z, this->secondCornerVoxelCoords.z);

			// Execute the command to edit the entire block
			this->getEditor()->getEditorCommandsManager()->execute<SetVoxelsEditorCommand>(this->getEditor()->getVoxelGrid(),
																						   selectedBlockAABBMinCoords,
																						   selectedBlockAABBMaxCoords,
																						   this->getEditor()->getCurrentBrushVoxelData());

			// The block has been edited, so clear both corner voxels
			this->firstCornerVoxelSelected = false;
		}
		else
		{
			// The first block edit voxel has been selected
			this->firstCornerVoxelCoords = this->currentFocusVoxelCoords;
			this->firstCornerVoxelSelected = true;
		}
	}

	void BlockEditTool::onMouseScrollWheelRotated(Int pScrollWheelRotationDelta)
	{
		// If mouse look is enabled, then the focus voxel distance should not be edited
		if(this->getEditor()->getCamera()->getIsMouseLookEnabled())
		{
			return;
		}

		// Increase or decrease the foxus voxel distance based on the amount the scroll wheel has rotated
		this->currentFocusVoxelDistance += pScrollWheelRotationDelta;
	}
}