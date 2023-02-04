#ifndef FORGE_CL_PROGRAM_INTERSECTION_TESTS_H
#define FORGE_CL_PROGRAM_INTERSECTION_TESTS_H

#include "ForgeData/OpenCL/Include/Voxel.cl"
#include "ForgeData/OpenCL/Include/VoxelUtility.cl"
#include "../Forge/ForgeData/OpenCL/Include/Ray.cl"
#include "../Forge/ForgeData/OpenCL/Include/IntersectionTests.cl"

struct RayAndVoxelOctreeIntersectionData
{
	struct Voxel voxel;
	float3 point;
	float2 uvCoords;
	float3 normal;
};

bool rayAndVoxelOctreeIntersectionTest(struct Ray pRay,
									   __global struct Voxel *pVoxelOctreeNodesArray,
									   uint pVoxelOctreeDepth,
									   long3 pVoxelGridMinCoords,
									   long3 pVoxelGridMaxCoords,
									   float pMaxRayDistance,
									   int pIsOctreeVisualizationEnabled,
									   struct RayAndVoxelOctreeIntersectionData *pIntersectionData)
{
	// This AABB that represents the bounds of the voxel grid
	struct AABB voxelGridAABB;
	// Calculate the AABB that represents the bounds of the voxel grid
	{
		voxelGridAABB.minPoint = convert_float3(pVoxelGridMinCoords) - 0.5f;
		voxelGridAABB.maxPoint = convert_float3(pVoxelGridMaxCoords) + 0.5f;
	}

	// Check if the origin of the ray is outside the voxel grid AABB, and handle that case as necessary
	{
		// Check if the origin of the ray is outside the voxel grid AABB
		if(pRay.origin.x < voxelGridAABB.minPoint.x || pRay.origin.x > voxelGridAABB.maxPoint.x ||
		   pRay.origin.y < voxelGridAABB.minPoint.y || pRay.origin.y > voxelGridAABB.maxPoint.y ||
		   pRay.origin.z < voxelGridAABB.minPoint.z || pRay.origin.z > voxelGridAABB.maxPoint.z)
		{
			// Check if the ray intersects the AABB of the voxel data
			struct RayAndAABBIntersectionData rayAndAABBIntersectionData;
			if(rayAndAABBIntersectionTest(pRay, voxelGridAABB, &rayAndAABBIntersectionData, MAXFLOAT))
			{
				// Move the origin of the ray to the point on the AABB at which the ray enters the AABB
				pRay.origin += pRay.direction * rayAndAABBIntersectionData.entryDistance;
			}
			else
			{
				// If the ray does not intersect the AABB of the voxel data, then the ray cannot not intersect any voxel in the voxel data
				return false;
			}
		}
	}

	// These are the iteration variables that are initialized at the start of the iteration and do not change during the iteration
	float3 rayDirectionSigns = sign(pRay.direction);

	// These are the iteration variables that are initialized at the start of the iteration and can change during the iteration
	uint currentNodeMortonCode = 0x00000001;
	uint currentLevel = 0;
	float nodeHalfScale = (voxelGridAABB.maxPoint.x - voxelGridAABB.minPoint.x) / 2.0f;
	float3 rayDistancesBetweenCoords = fabs(1.0f / pRay.direction) * (voxelGridAABB.maxPoint.x - voxelGridAABB.minPoint.x);
	bool hasCurrentNodeBeenExplored = false;
	float3 currentRayPosition = pRay.origin;
	float3 currentNodeCenter;
	float3 currentNodeParentCenter;
	currentNodeCenter.x = -0.5f;
	currentNodeCenter.y = -0.5f;
	currentNodeCenter.z = -0.5f;
	currentNodeParentCenter = currentNodeCenter;

	// Traverse the octree
	while(true)
	{
		// If the current node has been explored
		if(hasCurrentNodeBeenExplored)
		{
			// If the current level is 0
			if(currentLevel == 0)
			{
				// The ray has exited the octree without hitting any voxel, so return false
				return false;
			}
			// If the current level is not 0
			else
			{
				// Each component holds the distance that the ray needs to travel for the whole number part of the corresponding coordinate to reach its next value
				float3 rayDistancesToNextCoords;
				// Calculate the ray distances to the next coords
				{
					if(pRay.direction.x > 0.0f)
					{
						rayDistancesToNextCoords.x = (currentNodeCenter.x + nodeHalfScale - currentRayPosition.x) / (nodeHalfScale * 2.0f) * rayDistancesBetweenCoords.x;
					}
					else if(pRay.direction.x < 0.0f)
					{
						rayDistancesToNextCoords.x = (currentRayPosition.x - (currentNodeCenter.x - nodeHalfScale)) / (nodeHalfScale * 2.0f) * rayDistancesBetweenCoords.x;
					}
					else
					{
						rayDistancesToNextCoords.x = MAXFLOAT;
					}
	
					if(pRay.direction.y > 0.0f)
					{
						rayDistancesToNextCoords.y = (currentNodeCenter.y + nodeHalfScale - currentRayPosition.y) / (nodeHalfScale * 2.0f) * rayDistancesBetweenCoords.y;
					}
					else if(pRay.direction.y < 0.0f)
					{
						rayDistancesToNextCoords.y = (currentRayPosition.y - (currentNodeCenter.y - nodeHalfScale)) / (nodeHalfScale * 2.0f) * rayDistancesBetweenCoords.y;
					}
					else
					{
						rayDistancesToNextCoords.y = MAXFLOAT;
					}
	
					if(pRay.direction.z > 0.0f)
					{
						rayDistancesToNextCoords.z = (currentNodeCenter.z + nodeHalfScale - currentRayPosition.z) / (nodeHalfScale * 2.0f) * rayDistancesBetweenCoords.z;
					}
					else if(pRay.direction.z < 0.0f)
					{
						rayDistancesToNextCoords.z = (currentRayPosition.z - (currentNodeCenter.z - nodeHalfScale)) / (nodeHalfScale * 2.0f) * rayDistancesBetweenCoords.z;
					}
					else
					{
						rayDistancesToNextCoords.z = MAXFLOAT;
					}
				}

				float3 nextNodeCenter = currentNodeCenter;
				float3 nextRayPosition;
				// Calculate the center of the next node at this level and the next ray position
				{
					if(rayDistancesToNextCoords.x < rayDistancesToNextCoords.y)
					{
						if(rayDistancesToNextCoords.x < rayDistancesToNextCoords.z)
						{
							nextNodeCenter.x += nodeHalfScale * 2.0f * rayDirectionSigns.x;
							nextRayPosition = currentRayPosition + pRay.direction * rayDistancesToNextCoords.x;
						}
						else
						{
							nextNodeCenter.z += nodeHalfScale * 2.0f * rayDirectionSigns.z;
							nextRayPosition = currentRayPosition + pRay.direction * rayDistancesToNextCoords.z;
						}
					}
					else
					{
						if(rayDistancesToNextCoords.y < rayDistancesToNextCoords.z)
						{
							nextNodeCenter.y += nodeHalfScale * 2.0f * rayDirectionSigns.y;
							nextRayPosition = currentRayPosition + pRay.direction * rayDistancesToNextCoords.y;
						}
						else
						{
							nextNodeCenter.z += nodeHalfScale * 2.0f * rayDirectionSigns.z;
							nextRayPosition = currentRayPosition + pRay.direction * rayDistancesToNextCoords.z;
						}
					}
				}

				uint nextNodeIndex = 0;
				// Calculate the index of the next node
				{
					if(nextNodeCenter.x > currentNodeParentCenter.x)
					{
						nextNodeIndex |= 0x00000001;
					}
					if(nextNodeCenter.y > currentNodeParentCenter.y)
					{
						nextNodeIndex |= 0x00000002;
					}
					if(nextNodeCenter.z > currentNodeParentCenter.z)
					{
						nextNodeIndex |= 0x00000004;
					}
				}

				// If the next node the ray enters at the current level is a child of the parent of the current node
				if((currentNodeMortonCode & 0x00000007) != nextNodeIndex)
				{
					// Move to the next node at the current level that the ray enters
					{
						currentNodeMortonCode = (currentNodeMortonCode & 0xfffffff8) | nextNodeIndex;

						currentNodeCenter = nextNodeCenter;

						currentRayPosition = nextRayPosition;

						// Mark the current node at the current level as not fully explored
						hasCurrentNodeBeenExplored = false;
					}
				}
				// If the next node the ray enters at the current level is not a child of the parent of the current node
				else
				{
					// Step up a certain number of levels
					{
						// Step up to the next lowest level from which uniform grid traversal is possible
						// TODO: Can this be made faster, by removing the loop?
						{
							do
							{
								uint currentChildIndex = currentNodeMortonCode & 0x00000007;
								if(currentChildIndex & 0x00000001)
								{
									currentNodeCenter.x -= nodeHalfScale;
								}
								else
								{
									currentNodeCenter.x += nodeHalfScale;
								}

								if(currentChildIndex & 0x00000002)
								{
									currentNodeCenter.y -= nodeHalfScale;
								}
								else
								{
									currentNodeCenter.y += nodeHalfScale;
								}

								if(currentChildIndex & 0x00000004)
								{
									currentNodeCenter.z -= nodeHalfScale;
								}
								else
								{
									currentNodeCenter.z += nodeHalfScale;
								}

								currentNodeMortonCode >>= 3;
								--currentLevel;
								nodeHalfScale *= 2.0f;
								rayDistancesBetweenCoords *= 2.0f;
							}
							while(currentLevel != 0 && (currentNodeMortonCode & 0x00000007) == nextNodeIndex);

							// Calculate the center of the parent of the current node
							{
								uint currentChildIndex = currentNodeMortonCode & 0x00000007;
								currentNodeParentCenter = currentNodeCenter;
								if(currentChildIndex & 0x00000001)
								{
									currentNodeParentCenter.x -= nodeHalfScale;
								}
								else
								{
									currentNodeParentCenter.x += nodeHalfScale;
								}

								if(currentChildIndex & 0x00000002)
								{
									currentNodeParentCenter.y -= nodeHalfScale;
								}
								else
								{
									currentNodeParentCenter.y += nodeHalfScale;
								}

								if(currentChildIndex & 0x00000004)
								{
									currentNodeParentCenter.z -= nodeHalfScale;
								}
								else
								{
									currentNodeParentCenter.z += nodeHalfScale;
								}
							}

							hasCurrentNodeBeenExplored = true;
						}
					}
				}
			}
		}
		// If the current node has not been explored
		else
		{
			// If the current level is (pVoxelOctreeDepth - 1), which means it is the leaf nodes level
			if(currentLevel == (pVoxelOctreeDepth - 1))
			{
				// If the current leaf node has a non empty voxel
				if(pVoxelOctreeNodesArray[currentNodeMortonCode].voxelID)
				{
					// A voxel has been hit, so fill the intersection data and return true
					{
						pIntersectionData->voxel = pVoxelOctreeNodesArray[currentNodeMortonCode];
						pIntersectionData->point = currentRayPosition;

						// Calculate the uv coordinates and normals
						// TODO: Is there a better and/or faster way to do this?
						{
							struct AABB voxelAABB;
							voxelAABB.minPoint = currentNodeCenter + (float3)(-0.5f, -0.5f, -0.5f);
							voxelAABB.maxPoint = currentNodeCenter + (float3)(0.5f, 0.5f, 0.5f);
							struct RayAndAABBIntersectionData rayAndVoxelAABBIntersectionData;
							if(rayAndAABBIntersectionTest(pRay, voxelAABB, &rayAndVoxelAABBIntersectionData, MAXFLOAT))
							{
								pIntersectionData->normal = rayAndVoxelAABBIntersectionData.normal;

								if(fabs(rayAndVoxelAABBIntersectionData.normal.x) > 0.5f)
								{
									pIntersectionData->uvCoords.x = (currentRayPosition.z - voxelAABB.minPoint.z) / 1.0f;
									pIntersectionData->uvCoords.y = (currentRayPosition.y - voxelAABB.minPoint.y) / 1.0f;
								}
								else if(fabs(rayAndVoxelAABBIntersectionData.normal.y) > 0.5f)
								{
									pIntersectionData->uvCoords.x = (currentRayPosition.x - voxelAABB.minPoint.x) / 1.0f;
									pIntersectionData->uvCoords.y = (currentRayPosition.z - voxelAABB.minPoint.z) / 1.0f;
								}
								else if(fabs(rayAndVoxelAABBIntersectionData.normal.z) > 0.5f)
								{
									pIntersectionData->uvCoords.x = (currentRayPosition.x - voxelAABB.minPoint.x) / 1.0f;
									pIntersectionData->uvCoords.y = (currentRayPosition.y - voxelAABB.minPoint.y) / 1.0f;
								}
							}
							else
							{
								pIntersectionData->normal = (float3)(0.0f, 0.0f, 0.0f);
								pIntersectionData->uvCoords = (float2)(0.0f, 0.0f);
							}
						}

						return true;
					}
				}
				// If the current leaf node has the empty voxel
				else
				{
					// Mark the current node at the current level as explored
					hasCurrentNodeBeenExplored = true;
				}
			}
			// If the current level is not (pVoxelOctreeDepth - 1), which means it is not the leaf nodes level
			else
			{
				// If the current node has any children
				if(pVoxelOctreeNodesArray[currentNodeMortonCode].voxelID)
				{
					// TODO: This debug draws the octree nodes. Can this be done in a better way?
					{
						if(pIsOctreeVisualizationEnabled)
						{
							int numberOfCoordsInBorderRange = 0;
							if(fabs(currentNodeCenter.x - currentRayPosition.x) > nodeHalfScale - 0.1f)
							{
								++numberOfCoordsInBorderRange;
							}
							if(fabs(currentNodeCenter.y - currentRayPosition.y) > nodeHalfScale - 0.1f)
							{
								++numberOfCoordsInBorderRange;
							}
							if(fabs(currentNodeCenter.z - currentRayPosition.z) > nodeHalfScale - 0.1f)
							{
								++numberOfCoordsInBorderRange;
							}
							if(numberOfCoordsInBorderRange > 1)
							{
								pIntersectionData->voxel = pVoxelOctreeNodesArray[0];
								pIntersectionData->point = currentRayPosition;
								return true;
							}
						}
					}

					// Step down to the next lower level node, which is one of the child nodes of the current node
					{
						uint childNodeIndex = 0;
						currentNodeParentCenter = currentNodeCenter;

						if(currentRayPosition.x > currentNodeCenter.x)
						{
							childNodeIndex |= 0x00000001;
							currentNodeCenter.x += nodeHalfScale / 2.0f;
						}
						else
						{
							currentNodeCenter.x -= nodeHalfScale / 2.0f;
						}

						if(currentRayPosition.y > currentNodeCenter.y)
						{
							childNodeIndex |= 0x00000002;
							currentNodeCenter.y += nodeHalfScale / 2.0f;
						}
						else
						{
							currentNodeCenter.y -= nodeHalfScale / 2.0f;
						}

						if(currentRayPosition.z > currentNodeCenter.z)
						{
							childNodeIndex |= 0x00000004;
							currentNodeCenter.z += nodeHalfScale / 2.0f;
						}
						else
						{
							currentNodeCenter.z -= nodeHalfScale / 2.0f;
						}

						currentNodeMortonCode = (currentNodeMortonCode << 3) | childNodeIndex;
						
						++currentLevel;
						nodeHalfScale /= 2.0f;
						rayDistancesBetweenCoords /= 2.0f;

						// Mark the current node at the level just stepped down to as not explored
						hasCurrentNodeBeenExplored = false;
					}
				}
				// If the current node does not have any children
				else
				{
					// Mark the current node at the current level as explored
					hasCurrentNodeBeenExplored = true;
				}
			}
		}
	}

	// The code in the loop above should always return with the correct value, but this return statement is here just in case
	return false;
}

#endif