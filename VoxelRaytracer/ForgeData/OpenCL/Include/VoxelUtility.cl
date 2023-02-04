#ifndef FORGE_CL_VOXEL_UTILITY_H
#define FORGE_CL_VOXEL_UTILITY_H

uint part1By2For32BitInteger(uint pValue)
{
	// pValue used bits are now 00000000000000000000001111111111

	pValue = (pValue ^ (pValue << 16)) & 0x030000ff;
	// pValue used bits are now 00000011000000000000000011111111

	pValue = (pValue ^ (pValue << 8)) &  0x0300f00f;
	// pValue used bits are now 00000011000000001111000000001111

	pValue = (pValue ^ (pValue << 4)) &  0x030c30c3;
	// pValue used bits are now 00000011000011000011000011000011

	pValue = (pValue ^ (pValue << 2)) &  0x09249249;
	// pValue used bits are now 00001001001001001001001001001001

	return pValue;
}

uint getMortonCodeForVoxelCoords(uint3 pRemappedVoxelCoords,
								 uint pVoxelOctreeDepth)
{
	return (part1By2For32BitInteger(pRemappedVoxelCoords.z) << 2) |
		   (part1By2For32BitInteger(pRemappedVoxelCoords.y) << 1) |
		   part1By2For32BitInteger(pRemappedVoxelCoords.x) |
		   (1 << ((pVoxelOctreeDepth - 1) * 3));
}

#endif