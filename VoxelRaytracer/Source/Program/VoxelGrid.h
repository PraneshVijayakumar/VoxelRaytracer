#ifndef FORGE_VOXEL_GRID_H
#define FORGE_VOXEL_GRID_H

#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/NumericTypeInfo.h>
#include "VoxelData.h"
#include <Forge/Source/Core/Math/AABB.h>
#include <Forge/Source/Core/Math/IntegerVector3.h>
#include "VoxelGrid.h"
#include <Forge/Source/Core/Event.h>
#include <Forge/Source/Core/Containers/ResizableArray.h>
#include <Forge/Source/Core/Utility.h>
#include <Forge/Source/Core/Logging.h>
#include <Forge/Source/Platform/Windows/Threading/Mutex.h>

namespace forge
{
	// A 3D grid of voxels
	class VoxelGrid
	{
	public:

		using VoxelCoordType = Int32;

		VoxelGrid(VoxelCoordType pOctreeDepth);

		UInt32 getOctreeDepth() const;

		/// <summary>
		/// Get the coordinates of the bottom left voxel
		/// </summary>
		/// <returns>The coordinates of the bottom left voxel</returns>
		IntegerVector3<VoxelCoordType> getMinVoxelCoords() const;

		/// <summary>
		/// Get the coordinates of the top right voxel
		/// </summary>
		/// <returns>The coordinates of the top right voxel</returns>
		IntegerVector3<VoxelCoordType> getMaxVoxelCoords() const;

		VoxelCoordType getSideLength() const;

		/// <summary>
		/// Get the voxel data for the voxel at the given coordinates
		/// </summary>
		/// <param name="voxelCoords">The coordinates of the voxel</param>
		/// <returns>The voxel data</returns>
		VoxelData getVoxelData(const IntegerVector3<VoxelCoordType> &pVoxelCoords) const;

		/// <summary>
		/// Set the voxel data for the voxel at the given coordinates
		/// </summary>
		/// <param name="voxelCoords">The coordinates of the voxel</param>
		/// <param name="voxelDatum">The new voxel datum for the voxel</param>
		void setVoxelData(const IntegerVector3<VoxelCoordType> &pVoxelCoords, const VoxelData &pVoxelData);

		/// <summary>
		/// Checks if the given voxel coordinates are within the bounds of the voxel grid
		/// </summary>
		/// <param name="pVoxelCoordinates">The voxel coordinates to check</param>
		/// <returns>True if the given voxel coordinates are within the bounds of the voxel grid, false otherwise</returns>
		bool areCoordinatesWithinBounds(const IntegerVector3<VoxelCoordType> &pVoxelCoordinates) const;

		// Get the AABB of the voxel grid. Any point inside this AABB will be intersect a voxel
		AABB getAABB() const;

		ResizableArray<VoxelData>* getVoxelDataArray();

		const ResizableArray<VoxelData>* getVoxelDataArray() const;

		/// <summary>
		/// Converts 3D voxel coordinates into an index that can be used on a contiguous voxel data array
		/// </summary>
		/// <param name="xCoord">The x coordinate of the voxel</param>
		/// <param name="yCoord">The y coordinate of the voxel</param>
		/// <param name="zCoord">The z coordinate of the voxel</param>
		/// <returns>The index of the element in the voxel data array the represents the voxel with the given coordinates</returns>
		SizeType convertVoxelCoordsToArrayIndex(const IntegerVector3<VoxelCoordType> &pVoxelCoords) const;

		Event<UInt32, UInt8> onVoxelOctreeNodeUpdated;

	private:

		UInt32 getMortonCodeForVoxelCoords(const IntegerVector3<VoxelCoordType> &pVoxelCoords) const;

		UInt32 part1By2For32BitInteger(UInt32 pValue) const;

		// The hash table that holds the data for all the voxels
		ResizableArray<VoxelData> voxelDataArray;
		const UInt32 octreeDepth;
		// The coordinates of the bottom left voxel
		IntegerVector3<VoxelCoordType> minVoxelCoords;
		// The coordinates of the top right voxel
		IntegerVector3<VoxelCoordType> maxVoxelCoords;
		// The lengths of the voxel grid in all the dimension
		VoxelCoordType sideLength;
		// The AABB of the voxel grid. Any point inside this AABB will be intersect a voxel
		AABB aabb;
		Mutex mutex;
	};

	inline
	UInt32 VoxelGrid::getOctreeDepth() const
	{
		return this->octreeDepth;
	}

	inline
	IntegerVector3<VoxelGrid::VoxelCoordType> VoxelGrid::getMinVoxelCoords() const
	{
		return this->minVoxelCoords;
	}

	inline
	IntegerVector3<VoxelGrid::VoxelCoordType> VoxelGrid::getMaxVoxelCoords() const
	{
		return this->maxVoxelCoords;
	}

	inline
	VoxelGrid::VoxelCoordType VoxelGrid::getSideLength() const
	{
		return this->sideLength;
	}

	inline
	VoxelData VoxelGrid::getVoxelData(const IntegerVector3<VoxelCoordType> &pVoxelCoords) const
	{
		return this->voxelDataArray[this->convertVoxelCoordsToArrayIndex(pVoxelCoords)];
	}

	inline
	bool VoxelGrid::areCoordinatesWithinBounds(const IntegerVector3<VoxelCoordType> &pVoxelCoordinates) const
	{
		return pVoxelCoordinates.x >= this->minVoxelCoords.x && pVoxelCoordinates.x <= this->maxVoxelCoords.x &&
			   pVoxelCoordinates.y >= this->minVoxelCoords.y && pVoxelCoordinates.y <= this->maxVoxelCoords.y &&
			   pVoxelCoordinates.z >= this->minVoxelCoords.z && pVoxelCoordinates.z <= this->maxVoxelCoords.z;
	}

	inline
	AABB VoxelGrid::getAABB() const
	{
		return this->aabb;
	}

	inline
	ResizableArray<VoxelData>* VoxelGrid::getVoxelDataArray()
	{
		return &this->voxelDataArray;
	}

	inline
	const ResizableArray<VoxelData>* VoxelGrid::getVoxelDataArray() const
	{
		return &this->voxelDataArray;
	}

	inline
	SizeType VoxelGrid::convertVoxelCoordsToArrayIndex(const IntegerVector3<VoxelCoordType> &pVoxelCoords) const
	{
		// Do nothing if the givel voxel coordinates are out of bounds
		if(!this->areCoordinatesWithinBounds(pVoxelCoords))
		{
			FORGE_ERROR_LOG("The voxel coordinates given to the convertVoxelCoordsToArrayIndex function are out of bounds");
			return 0;
		}

		return this->getMortonCodeForVoxelCoords(pVoxelCoords);
	}

	inline
	UInt32 VoxelGrid::getMortonCodeForVoxelCoords(const IntegerVector3<VoxelCoordType> &pVoxelCoords) const
	{
		IntegerVector3<UInt32> remappedCoords;

		// Remapping the voxel coordinates so that they are positive, essentially converting them to the range of the unsigned integer that is the same size as VoxelCoordType.
		remappedCoords.x = static_cast<UInt32>(pVoxelCoords.x - this->minVoxelCoords.x);
		remappedCoords.y = static_cast<UInt32>(pVoxelCoords.y - this->minVoxelCoords.y);
		remappedCoords.z = static_cast<UInt32>(pVoxelCoords.z - this->minVoxelCoords.z);

		return (this->part1By2For32BitInteger(remappedCoords.z) << 2) |
			   (this->part1By2For32BitInteger(remappedCoords.y) << 1) |
			   this->part1By2For32BitInteger(remappedCoords.x) |
			   bitwiseLeftShiftInteger<UInt32>(1, (this->octreeDepth - 1) * 3);
	}

	inline
	UInt32 VoxelGrid::part1By2For32BitInteger(UInt32 pValue) const
	{
		// TODO: The bitwise shift operators are used directly in this function. Should use the bitwise shift utility functions instead
		// pValue used bits are now 00000000000000000000001111111111

		pValue = (pValue ^ (pValue << 16)) & 0b00000011000000000000000011111111;
		// pValue used bits are now 00000011000000000000000011111111

		pValue = (pValue ^ (pValue << 8)) &  0b00000011000000001111000000001111;
		// pValue used bits are now 00000011000000001111000000001111

		pValue = (pValue ^ (pValue << 4)) &  0b00000011000011000011000011000011;
		// pValue used bits are now 00000011000011000011000011000011

		pValue = (pValue ^ (pValue << 2)) &  0b00001001001001001001001001001001;
		// pValue used bits are now 00001001001001001001001001001001

		return pValue;
	}
}

#endif