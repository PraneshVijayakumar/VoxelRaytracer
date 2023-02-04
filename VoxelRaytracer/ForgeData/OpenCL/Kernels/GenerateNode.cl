#include "../Forge/ForgeData/OpenCL/Include/Random.cl"
#include "ForgeData/OpenCL/Include/Voxel.cl"
#include "ForgeData/OpenCL/Include/VoxelUtility.cl"

__kernel void generateNodeKernel(__global struct Voxel *pVoxelArray,
								 uint pVoxelOctreeDepth)
{
	// Get the coordinates of the node for which this kernel instance is being executed for remmaped to the range (0, gridSize - 1)
	uint3 remappedVoxelCoords;
	remappedVoxelCoords.x = get_global_id(0);
	remappedVoxelCoords.y = get_global_id(1);
	remappedVoxelCoords.z = get_global_id(2);

	uint nodeMortonCode = getMortonCodeForVoxelCoords(remappedVoxelCoords, pVoxelOctreeDepth);
	// For each of the children of the current node
	for(uint i = 0; i < 8; ++i)
	{
		// Calculate the morton code of the child node
		uint childNodeMortonCode = (nodeMortonCode << 3) | i;

		// If the child node exists or has any children
		if(pVoxelArray[childNodeMortonCode].voxelID)
		// Set the hasChild bit of the child to 1
		{
			pVoxelArray[nodeMortonCode].voxelID |= (1 << i);
		}
		// If the child node does not exist or have any children
		else
		// Set the hasChild bit of the child to 0
		{
			pVoxelArray[nodeMortonCode].voxelID &= ~(1 << i);
		}
	}
}