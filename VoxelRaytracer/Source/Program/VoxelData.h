#ifndef FORGE_VOXEL_DATA_H
#define FORGE_VOXEL_DATA_H

#include <Forge/Source/Core/Forge.h>

namespace forge
{
	// Represents a single voxel of any kind, including an empty voxel
	class VoxelData
	{
	public:

		bool getIsEmpty() const;

		UInt8 voxelID = 0;
	};

	inline
	bool VoxelData::getIsEmpty() const
	{
		return this->voxelID == 0;
	}
}

#endif