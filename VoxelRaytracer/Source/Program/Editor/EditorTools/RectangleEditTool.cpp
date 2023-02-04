#include "RectangleEditTool.h"
#include <VoxelRaytracer/Source/Program/Editor/Editor.h>
#include <VoxelRaytracer/Source/Program/ProgramIntersectionTests.h>
#include <VoxelRaytracer/Source/Program/Editor/EditorCommands/SetVoxelsEditorCommand.h>

namespace forge
{
	void RectangleEditTool::update()
	{
		// Update the current focus voxel coordinates
		this->updateCurrentFocusVoxelCoordinates();
	}

	void RectangleEditTool::getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs)
	{
		// If the tool is not active or mouse look is enabled, return
		if(this->getEditor()->getCamera()->getIsMouseLookEnabled())
		{
			return;
		}

		// If the first corner voxel has been selected, it is always displayed
		if(this->firstCornerVoxelSelected)
		{
			// Calculate the integer coordinates of the minimum and maximum corner voxels of the rectangular block
			IntegerVector3<VoxelGrid::VoxelCoordType> selectedBlockAABBMinCoords;
			selectedBlockAABBMinCoords.x = min<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.x, this->currentFocusVoxelCoords.x);
			selectedBlockAABBMinCoords.y = min<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.y, this->currentFocusVoxelCoords.y);
			selectedBlockAABBMinCoords.z = min<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.z, this->currentFocusVoxelCoords.z);
			IntegerVector3<VoxelGrid::VoxelCoordType> selectedBlockAABBMaxCoords;
			selectedBlockAABBMaxCoords.x = max<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.x, this->currentFocusVoxelCoords.x);
			selectedBlockAABBMaxCoords.y = max<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.y, this->currentFocusVoxelCoords.y);
			selectedBlockAABBMaxCoords.z = max<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.z, this->currentFocusVoxelCoords.z);

			DebugAABBData currentFirstVoxelDebugAABBData;
			currentFirstVoxelDebugAABBData.aabb.setMinPoint(Vector3(selectedBlockAABBMinCoords.x - 0.5f, selectedBlockAABBMinCoords.y - 0.5f, selectedBlockAABBMinCoords.z - 0.5f));
			currentFirstVoxelDebugAABBData.aabb.setMaxPoint(Vector3(selectedBlockAABBMaxCoords.x + 0.5f, selectedBlockAABBMaxCoords.y + 0.5f, selectedBlockAABBMaxCoords.z + 0.5f));
			currentFirstVoxelDebugAABBData.color.setRGBUint8(255, 0, 0);
			pDebugAABBs.append(currentFirstVoxelDebugAABBData);
		}
		else
		{
			// If the curent focus voxel exists and mouse look is not enabled
			if(this->currentFocusVoxelExists && !this->getEditor()->getCamera()->getIsMouseLookEnabled())
			{
				// Display the current focus voxel
				DebugAABBData currentFocusVoxelDebugAABBData;
				currentFocusVoxelDebugAABBData.aabb.setMinPoint(Vector3(this->currentFocusVoxelCoords.x - 0.5f, this->currentFocusVoxelCoords.y - 0.5f, this->currentFocusVoxelCoords.z - 0.5f));
				currentFocusVoxelDebugAABBData.aabb.setMaxPoint(Vector3(this->currentFocusVoxelCoords.x + 0.5f, this->currentFocusVoxelCoords.y + 0.5f, this->currentFocusVoxelCoords.z + 0.5f));
				currentFocusVoxelDebugAABBData.color.setRGBUint8(255, 0, 0);
				pDebugAABBs.append(currentFocusVoxelDebugAABBData);
			}
		}
	}

	void RectangleEditTool::activateImplementation()
	{
		this->getEditor()->getWindow()->getInput()->onMouseLeftButtonDown.addListener<RectangleEditTool, &RectangleEditTool::onMouseLeftButtonDown>(this);
		this->getEditor()->getWindow()->getInput()->onMouseCursorMoved.addListener<RectangleEditTool, &RectangleEditTool::onMouseCursorMoved>(this);
		this->getEditor()->getWindow()->getInput()->onMouseLeftButtonUp.addListener<RectangleEditTool, &RectangleEditTool::onMouseLeftButtonUp>(this);

		// Clear the corner voxels
		this->firstCornerVoxelSelected = false;

		// Update the current focus voxel coordinates
		this->updateCurrentFocusVoxelCoordinates();
	}

	void RectangleEditTool::deactivateImplementation()
	{
		this->getEditor()->getWindow()->getInput()->onMouseLeftButtonDown.removeListener<RectangleEditTool, &RectangleEditTool::onMouseLeftButtonDown>(this);
		this->getEditor()->getWindow()->getInput()->onMouseCursorMoved.removeListener<RectangleEditTool, &RectangleEditTool::onMouseCursorMoved>(this);
		this->getEditor()->getWindow()->getInput()->onMouseLeftButtonUp.removeListener<RectangleEditTool, &RectangleEditTool::onMouseLeftButtonUp>(this);
	}

	void RectangleEditTool::onMouseLeftButtonDown(Int pXCoord, Int pYCoord)
	{
		if(this->currentFocusVoxelExists)
		{
			// Set the first corner voxel coordinates to be the same as the the current focus voxel coordinates
			this->firstCornerVoxelCoords = this->currentFocusVoxelCoords;
			this->firstCornerVoxelSelected = true;
		}
	}

	void RectangleEditTool::onMouseCursorMoved(Int pXCoord, Int pYCoord)
	{
		// Update the current focus voxel coordinates
		//this->updateCurrentFocusVoxelCoordinates();
	}

	void RectangleEditTool::onMouseLeftButtonUp(Int pXCoord, Int pYCoord)
	{
		// If the first corner voxel has not been selected, the edit cannot be done
		if(!this->firstCornerVoxelSelected)
		{
			return;
		}

		// Set the second corner voxel coordinates to be the same as the the current focus voxel coordinates
		this->secondCornerVoxelCoords = this->currentFocusVoxelCoords;

		// Calculate the integer coordinates of the minimum and maximum corner voxels of the rectangular block
		IntegerVector3<VoxelGrid::VoxelCoordType> selectedBlockAABBMinCoords;
		selectedBlockAABBMinCoords.x = min<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.x, this->secondCornerVoxelCoords.x);
		selectedBlockAABBMinCoords.y = min<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.y, this->secondCornerVoxelCoords.y);
		selectedBlockAABBMinCoords.z = min<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.z, this->secondCornerVoxelCoords.z);
		IntegerVector3<VoxelGrid::VoxelCoordType> selectedBlockAABBMaxCoords;
		selectedBlockAABBMaxCoords.x = max<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.x, this->secondCornerVoxelCoords.x);
		selectedBlockAABBMaxCoords.y = max<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.y, this->secondCornerVoxelCoords.y);
		selectedBlockAABBMaxCoords.z = max<VoxelGrid::VoxelCoordType>(this->firstCornerVoxelCoords.z, this->secondCornerVoxelCoords.z);

		// Execute the editor command to edit the selected rectangular block of voxels
		this->getEditor()->getEditorCommandsManager()->execute<SetVoxelsEditorCommand>(this->getEditor()->getVoxelGrid(),
																					   selectedBlockAABBMinCoords,
																					   selectedBlockAABBMaxCoords,
																					   this->getEditor()->getCurrentBrushVoxelData());

		// The rectangular block has been edited, so clear both corner voxels
		this->firstCornerVoxelSelected = false;
	}

	void RectangleEditTool::updateCurrentFocusVoxelCoordinates()
	{
		// Get the camera ray for the pixel that the mouse cursor is currently over
		Ray cameraMouseCursorRay = this->getEditor()->getCamera()->getCameraRayForPixel(this->getEditor()->getWindow()->getInput()->getCurrentCursorXCoord(),
																						this->getEditor()->getWindow()->getInput()->getCurrentCursorYCoord(),
																						this->getEditor()->getWindow()->getResolutionWidth(),
																						this->getEditor()->getWindow()->getResolutionHeight());

		if(this->firstCornerVoxelSelected)
		{
			RayAndPlaneIntersectionData rayAndPlaneIntersectionData;
			if(rayAndPlaneIntersectionTest(cameraMouseCursorRay, this->currentFocusVoxelPlane, &rayAndPlaneIntersectionData))
			{
				this->currentFocusVoxelCoords.x = roundFloatToInteger<VoxelGrid::VoxelCoordType>(rayAndPlaneIntersectionData.point.x);
				this->currentFocusVoxelCoords.y = roundFloatToInteger<VoxelGrid::VoxelCoordType>(rayAndPlaneIntersectionData.point.y);
				this->currentFocusVoxelCoords.z = roundFloatToInteger<VoxelGrid::VoxelCoordType>(rayAndPlaneIntersectionData.point.z);

				// Calculate the integer coordinates of the voxel that the origin of the ray is in
				for(SizeType i = 0; i < 3; ++i)
				{
					// If the coord is the one for which the corresponding coord in the normal is 1.0f, then the coord will be 0.5f.
					// Just set the integer coordinate explicitly, in order to avoid floating point rounding error when rounding 0.5f to the nearest integer.
					// If the coord is not the one for which the normal is 1.0f, then it can be rounded to the nearest integer

					if(rayAndPlaneIntersectionData.normal[i] > 0.5f)
					{
						this->currentFocusVoxelCoords[i] = floorFloatToInteger<VoxelGrid::VoxelCoordType>(rayAndPlaneIntersectionData.point[i]);
					}
					else if(rayAndPlaneIntersectionData.normal[i] < -0.5f)
					{
						this->currentFocusVoxelCoords[i] = ceilingFloatToInteger<VoxelGrid::VoxelCoordType>(rayAndPlaneIntersectionData.point[i]);
					}
					else
					{
						this->currentFocusVoxelCoords[i] = roundFloatToInteger<VoxelGrid::VoxelCoordType>(rayAndPlaneIntersectionData.point[i]);
					}

					this->currentFocusVoxelCoords[i] = clamp<VoxelGrid::VoxelCoordType>(this->currentFocusVoxelCoords[i],
																						this->getEditor()->getVoxelGrid()->getMinVoxelCoords()[i],
																						this->getEditor()->getVoxelGrid()->getMaxVoxelCoords()[i]);
				}
			}
			else
			{
				this->currentFocusVoxelExists = false;
			}
		}
		else
		{
			RayAndVoxelDataIntersectionData rayAndVoxelDataIntersectionData;
			rayAndVoxelGridIntersectionTest(cameraMouseCursorRay, *this->getEditor()->getVoxelGrid(), rayAndVoxelDataIntersectionData);
			// If the current camera mouse cursor ray intersects a non-empty voxel
			if(rayAndVoxelDataIntersectionData.wasNonEmptyVoxelHit)
			{
				// Update the current focus voxel
				this->currentFocusVoxelCoords = rayAndVoxelDataIntersectionData.voxelCoords;
				this->currentFocusVoxelExists = true;
			}
			else
			{
				this->currentFocusVoxelExists = false;
			}

			this->currentFocusVoxelPlane = Plane(rayAndVoxelDataIntersectionData.normal, rayAndVoxelDataIntersectionData.point);
		}
	}
}