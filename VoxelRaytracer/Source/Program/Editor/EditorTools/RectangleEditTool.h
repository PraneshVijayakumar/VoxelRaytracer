#ifndef FORGE_RECTANGLE_EDIT_TOOL_H
#define FORGE_RECTANGLE_EDIT_TOOL_H

#include "EditorTool.h"
#include <VoxelRaytracer/Source/Program/VoxelGrid.h>
#include <Forge/Source/Core/Math/Plane.h>
#include <VoxelRaytracer/Source/Rendering/DebugAABBData.h>

namespace forge
{
	class RectangleEditTool : public EditorTool
	{
	public:

		RectangleEditTool(Editor *pEditor);

		virtual void update() override;

		virtual void getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs) override;

	protected:

		virtual void activateImplementation() override;

		virtual void deactivateImplementation() override;

	private:

		void onMouseLeftButtonDown(Int pXCoord, Int pYCoord);
		void onMouseCursorMoved(Int pXCoord, Int pYCoord);
		void onMouseLeftButtonUp(Int pXCoord, Int pYCoord);

		void updateCurrentFocusVoxelCoordinates();

		bool currentFocusVoxelExists;
		// The coordinates of the current focus voxel. The focus voxel is the one that will be chosen
		// as the first or rectangular second block edit voxel when the use clicks the left mouse button
		IntegerVector3<VoxelGrid::VoxelCoordType> currentFocusVoxelCoords;

		// Indicates whether the first corner voxel of the rectangular block to be edited has been selected
		bool firstCornerVoxelSelected;
		// The coordinates of the first corner voxel
		IntegerVector3<VoxelGrid::VoxelCoordType> firstCornerVoxelCoords;
		// The coordinates of the second corner voxel
		IntegerVector3<VoxelGrid::VoxelCoordType> secondCornerVoxelCoords;
		Plane currentFocusVoxelPlane;
	};

	inline
	RectangleEditTool::RectangleEditTool(Editor *pEditor) :
	EditorTool(pEditor)
	{
	}
}

#endif