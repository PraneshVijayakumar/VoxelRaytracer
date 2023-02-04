#include "SetVoxelsEditorCommand.h"

namespace forge
{
	void SetVoxelsEditorCommand::execute()
	{
		SizeType nextIndex = 0;
		for(VoxelGrid::VoxelCoordType x = this->setAreaMinCoords.x; x <= this->setAreaMaxCoords.x; ++x)
		{
			for(VoxelGrid::VoxelCoordType y = this->setAreaMinCoords.y; y <= this->setAreaMaxCoords.y; ++y)
			{
				for(VoxelGrid::VoxelCoordType z = this->setAreaMinCoords.z; z <= this->setAreaMaxCoords.z; ++z)
				{
					IntegerVector3<VoxelGrid::VoxelCoordType> integerVoxelCoords(x, y, z);
					this->previousAreaVoxelDataArray[nextIndex++] = this->voxelGrid->getVoxelData(integerVoxelCoords);
					this->voxelGrid->setVoxelData(integerVoxelCoords, this->voxelDataToSetWith);
				}
			}
		}
	}

	void SetVoxelsEditorCommand::undo()
	{
		SizeType nextIndex = 0;
		for(VoxelGrid::VoxelCoordType x = this->setAreaMinCoords.x; x <= this->setAreaMaxCoords.x; ++x)
		{
			for(VoxelGrid::VoxelCoordType y = this->setAreaMinCoords.y; y <= this->setAreaMaxCoords.y; ++y)
			{
				for(VoxelGrid::VoxelCoordType z = this->setAreaMinCoords.z; z <= this->setAreaMaxCoords.z; ++z)
				{
					this->voxelGrid->setVoxelData(IntegerVector3<VoxelGrid::VoxelCoordType>(x, y, z), this->previousAreaVoxelDataArray[nextIndex++]);
				}
			}
		}
	}
}