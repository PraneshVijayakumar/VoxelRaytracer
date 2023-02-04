#ifndef FORGE_SET_VOXELS_EDITOR_COMMAND_H
#define FORGE_SET_VOXELS_EDITOR_COMMAND_H

#include <Forge/Source/Core/Commands/Command.h>
#include <VoxelRaytracer/Source/Program/VoxelGrid.h>

namespace forge
{
	class SetVoxelsEditorCommand : public Command
	{
	public:

		SetVoxelsEditorCommand(VoxelGrid *pVoxelGrid,
							   IntegerVector3<VoxelGrid::VoxelCoordType> pSetAreaMinCoords,
							   IntegerVector3<VoxelGrid::VoxelCoordType> pSetAreaMaxCoords,
							   VoxelData pVoxelData);

		~SetVoxelsEditorCommand();

	private:

		virtual void execute() override;

		virtual void undo() override;

		VoxelGrid *voxelGrid;
		IntegerVector3<VoxelGrid::VoxelCoordType> setAreaMinCoords;
		IntegerVector3<VoxelGrid::VoxelCoordType> setAreaMaxCoords;
		VoxelData voxelDataToSetWith;
		VoxelData *previousAreaVoxelDataArray = nullptr;
	};

	inline
	SetVoxelsEditorCommand::SetVoxelsEditorCommand(VoxelGrid *pVoxelGrid,
												   IntegerVector3<VoxelGrid::VoxelCoordType> pSetAreaMinCoords,
												   IntegerVector3<VoxelGrid::VoxelCoordType> pSetAreaMaxCoords,
												   VoxelData pVoxelData) :
	voxelGrid(pVoxelGrid),
	setAreaMinCoords(pSetAreaMinCoords),
	setAreaMaxCoords(pSetAreaMaxCoords),
	voxelDataToSetWith(pVoxelData)
	{
		this->previousAreaVoxelDataArray = new VoxelData[(pSetAreaMaxCoords.x - pSetAreaMinCoords.x + 1) *
														 (pSetAreaMaxCoords.y - pSetAreaMinCoords.y + 1) *
														 (pSetAreaMaxCoords.z - pSetAreaMinCoords.z + 1)];
	}

	inline
	SetVoxelsEditorCommand::~SetVoxelsEditorCommand()
	{
		delete this->previousAreaVoxelDataArray;
	}
}

#endif