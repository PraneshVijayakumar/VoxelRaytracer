#include "VoxelGrid.h"
#include <Forge/Source/Core/Math/MathUtility.h>

namespace forge
{
	VoxelGrid::VoxelGrid(VoxelCoordType pOctreeDepth) :
	octreeDepth(pOctreeDepth)
	{
		this->sideLength = power<VoxelCoordType>(2, pOctreeDepth - 1);

		const VoxelCoordType halfSideLength = this->sideLength / 2;
		this->minVoxelCoords = IntegerVector3<VoxelCoordType>(-halfSideLength, -halfSideLength, -halfSideLength);
		this->maxVoxelCoords = IntegerVector3<VoxelCoordType>(halfSideLength - 1, halfSideLength - 1, halfSideLength - 1);

		this->aabb.setMinPoint(Vector3(static_cast<Float>(-halfSideLength) - 0.5f, static_cast<Float>(-halfSideLength) - 0.5f, static_cast<Float>(-halfSideLength) - 0.5f));
		this->aabb.setMaxPoint(Vector3(static_cast<Float>(halfSideLength) - 0.5f, static_cast<Float>(halfSideLength) - 0.5f, static_cast<Float>(halfSideLength) - 0.5f));

		// Allocate the voxel data array
		const SizeType sideLengthSizeType = static_cast<SizeType>(this->sideLength);
		this->voxelDataArray.setNumberOfElements(sideLengthSizeType * sideLengthSizeType * sideLengthSizeType * 2);
	}

	void VoxelGrid::setVoxelData(const IntegerVector3<VoxelCoordType> &pVoxelCoords, const VoxelData &pVoxelData)
	{
		// If the voxel being placed is empty
		if(pVoxelData.getIsEmpty())
		{
			// Get the Morton code for the voxel coords
			UInt32 voxelCoordsMortonCode = this->getMortonCodeForVoxelCoords(pVoxelCoords);

			// If the voxel is already the empty voxel, nothing needs to be done
			if(this->voxelDataArray[voxelCoordsMortonCode].voxelID == 0)
			{
				return;
			}

			// Set the voxel's data in the voxel data array.
			// Set the leaf node bit of the Morton key and use that number as the index for the voxel
			this->voxelDataArray[voxelCoordsMortonCode] = pVoxelData;

			// Update the bit in the parent node's bitfield that indicates whether this child node exists or not
			UInt32 mortonCodeOfParentNodeOfVoxel = bitwiseRightShiftInteger<UInt32>(voxelCoordsMortonCode, 3);
			UInt8 childNodeIndex = static_cast<UInt8>(voxelCoordsMortonCode & 0x00000007);
			this->voxelDataArray[mortonCodeOfParentNodeOfVoxel].voxelID &= ~bitwiseLeftShiftInteger<UInt8>(1, childNodeIndex);
			this->onVoxelOctreeNodeUpdated.invoke(mortonCodeOfParentNodeOfVoxel, this->voxelDataArray[mortonCodeOfParentNodeOfVoxel].voxelID);

			// Update all the necessary child node bitfields of the octree node hierarchy to reflect the voxel that was updated
			{
				// This is the morton code for the current octree node
				UInt32 currentOctreeNodeMortonCode = mortonCodeOfParentNodeOfVoxel;
				// Each iteration of this loop processes one node.
				// The iteration happens from the bottom of the hierarchy to the top, starting from level 7 of the octree
				for(UInt32 i = 0; i < this->octreeDepth - 2; ++i)
				{
					// This is the Morton key of the parent node of the current node
					UInt32 parentNodeMortonCode = bitwiseRightShiftInteger<UInt32>(currentOctreeNodeMortonCode, 3);

					if(this->voxelDataArray[currentOctreeNodeMortonCode].voxelID == 0)
					{
						UInt8 childNodeIndex = static_cast<UInt8>(currentOctreeNodeMortonCode & 0x00000007);
						this->voxelDataArray[parentNodeMortonCode].voxelID &= ~bitwiseLeftShiftInteger<UInt8>(1, childNodeIndex);
						this->onVoxelOctreeNodeUpdated.invoke(parentNodeMortonCode, this->voxelDataArray[parentNodeMortonCode].voxelID);
					}
					else
					{
						break;
					}

					// The current node for the next iteration is the parent node of this iteration
					currentOctreeNodeMortonCode = parentNodeMortonCode;
				}
			}
		}
		// If the voxel being placed is non-empty
		else
		{
			// Get the Morton key for the voxel coords
			UInt32 voxelCoordsMortonCode = this->getMortonCodeForVoxelCoords(pVoxelCoords);

			// Set the voxel's data in the voxel data array.
			// Set the leaf node bit of the Morton key and use that number as the index for the voxel
			this->voxelDataArray[voxelCoordsMortonCode] = pVoxelData;

			// Update all the necessary child node bitfields of the octree node hierarchy to reflect the voxel that was updated
			{
				// This is the morton code for the current octree node
				UInt32 currentOctreeNodeMortonCode = voxelCoordsMortonCode;
				// Each iteration of this loop processes one parent node. The iteration happens from the bottom of the hierarchy to the top
				for(UInt32 i = 0; i < this->octreeDepth - 1; ++i)
				{
					// This is the Morton key of the parent node of the current node
					UInt32 parentNodeMortonKey = bitwiseRightShiftInteger<UInt32>(currentOctreeNodeMortonCode, 3);
					// This indicates the position of the bit in the parent node's bitfield that indicates whether this child node exists or not
					UInt8 childNodeIndex = static_cast<UInt8>(currentOctreeNodeMortonCode & 0x00000007);
					// This is a bitfield with the bit that indicates this child node set to true
					UInt8 childNodeBitfieldPositionValue = bitwiseLeftShiftInteger<UInt8>(1, childNodeIndex);

					// If the parent node already exists
					if(this->voxelDataArray[parentNodeMortonKey].voxelID & childNodeBitfieldPositionValue)
					{
						// If the parent node already exists, then a path from the top of the hierarchy to the current node must exist.
						// So there is no need to update the bitfields of any more nodes
						break;
					}
					else
					{
						// Update the bit in the parent node's bitfield that indicates whether this child node exists or not
						this->voxelDataArray[parentNodeMortonKey].voxelID |= childNodeBitfieldPositionValue;
						this->onVoxelOctreeNodeUpdated.invoke(parentNodeMortonKey, this->voxelDataArray[parentNodeMortonKey].voxelID);
					}

					// The current node for the next iteration is the parent node of this iteration
					currentOctreeNodeMortonCode = parentNodeMortonKey;
				}
			}
		}
	}
}