#include "../Forge/ForgeData/OpenCL/Include/Random.cl"
#include "ForgeData/OpenCL/Include/Voxel.cl"
#include "ForgeData/OpenCL/Include/VoxelUtility.cl"

struct Voxel generateVoxel(const uint pSeed, int3 pVoxelCoords, float pOffset)
{
	float3 floatCoords;
	floatCoords.x = (float)(pVoxelCoords.x) + pOffset;
	floatCoords.y = (float)(pVoxelCoords.y) + pOffset;
	floatCoords.z = (float)(pVoxelCoords.z) + pOffset;
	float heightValue = valueNoise3D(pSeed, floatCoords / 80.0f);
	if(heightValue <= (float)(0.2f))
	{
		struct Voxel voxel;
		voxel.voxelID = 1;
		return voxel;
	}
	else
	{
		struct Voxel voxel;
		voxel.voxelID = 0;
		return voxel;
	}
}

// This kernel gets executed for every voxel in the voxel grid. The global ID in the first, second and third dimensions correspond
// to the integer coordinates of the voxel the current work item is being executed for
__kernel void generateVoxelKernel(__global struct Voxel *pVoxelArray,
								  uint pVoxelOctreeDepth, // An array in which each element represents the output data for a pixel
								  float pOffset) 
{
	// Get the coordinates of the voxel for which this kernel instance is being executed for remmaped to the range (0, voxelGridSize - 1)
	uint3 remappedVoxelCoords;
	remappedVoxelCoords.x = get_global_id(0);
	remappedVoxelCoords.y = get_global_id(1);
	remappedVoxelCoords.z = get_global_id(2);

	// Get half the resolution of the voxel grid
	int3 halfVoxelGridSize;
	halfVoxelGridSize.x = get_global_size(0) / 2;
	halfVoxelGridSize.y = get_global_size(1) / 2;
	halfVoxelGridSize.z = get_global_size(2) / 2;

	// The actual coordinates of the current voxel
	int3 voxelCoords = convert_int3(remappedVoxelCoords) - halfVoxelGridSize;

	// Generate the voxel and put it into its position in the voxel data array
	pVoxelArray[getMortonCodeForVoxelCoords(remappedVoxelCoords, pVoxelOctreeDepth)] = generateVoxel(1, voxelCoords, pOffset);
}