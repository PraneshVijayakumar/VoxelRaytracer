#ifndef FORGE_BLOCK_EDIT_TOOL_H
#define FORGE_BLOCK_EDIT_TOOL_H

#include "EditorTool.h"
#include <VoxelRaytracer/Source/Program/VoxelGrid.h>
#include <VoxelRaytracer/Source/Rendering/DebugAABBData.h>

namespace forge
{
	// Edits a block of voxels that is shaped like a cuboid.
	// The block is selected by selecting the minimum and maximum corner voxels of the block.
	// The two corner voxels can be selected in any order
	class BlockEditTool : public EditorTool
	{
	public:

		BlockEditTool(Editor *pEditor);

		virtual void update() override;

		virtual void getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs) override;

	protected:

		virtual void activateImplementation() override;

		virtual void deactivateImplementation() override;

	private:

		void onMouseLeftButtonUp(Int pXCoord, Int pYCoord);
		void onMouseScrollWheelRotated(Int pScrollWheelRotationDelta);

		// The number of voxels, excluding the voxel the camera is in,
		// that the current mouse cursor camera ray must traverse to get to the current focus voxel
		VoxelGrid::VoxelCoordType currentFocusVoxelDistance;
		// The coordinates of the current focus voxel. The focus voxel is the one that will be chosen
		// as the first or second block edit voxel when the use clicks the left mouse button
		IntegerVector3<VoxelGrid::VoxelCoordType> currentFocusVoxelCoords;

		// Indicates whether the first corner voxel of the block to be edited has been selected
		bool firstCornerVoxelSelected;
		// The coordinates of the first corner voxel
		IntegerVector3<VoxelGrid::VoxelCoordType> firstCornerVoxelCoords;
		// The coordinates of the second corner voxel
		IntegerVector3<VoxelGrid::VoxelCoordType> secondCornerVoxelCoords;
	};

	inline
	BlockEditTool::BlockEditTool(Editor *pEditor) :
	EditorTool(pEditor)
	{
	}
}

#endif