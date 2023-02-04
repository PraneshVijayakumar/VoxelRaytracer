#include "ProgramIntersectionTests.h"
#include <Forge/Source/Core/NumericTypeInfo.h>
#include <Forge/Source/Core/Math/MathUtility.h>
#include <Forge/Source/Core/Utility.h>

namespace forge
{
	bool rayAndVoxelGridIntersectionTest(Ray pRay, const VoxelGrid &pVoxelGrid, RayAndVoxelDataIntersectionData &pIntersectionData, bool pShouldStopOnNonEmptyVoxelHit, SizeType pMaximumNumberOfVoxelsToTraverse)
	{
		// The integer coordinates of the current voxel
		IntegerVector3<VoxelGrid::VoxelCoordType> currentVoxelCoords;

		// Calculate the starting value for the current voxel coordinates.
		// They are just the coordinates of the voxel the origin of the ray is in.
		// The starting coordinates are calculated differently depending on whether the origin of the ray is inside the voxel volume or not
		if(pVoxelGrid.getAABB().isPointInside(pRay.getOrigin()))
		{
			for(SizeType i = 0; i < 3; ++i)
			{
				currentVoxelCoords[i] = roundFloatToInteger<VoxelGrid::VoxelCoordType>(pRay.getOrigin()[i]);
			}
		}
		else
		{
			// Check if the ray intersects the AABB of the voxel grid
			RayAndAABBIntersectionData rayAndAABBIntersectionData;
			if(!rayAndAABBIntersectionTest(Ray(pRay.getOrigin(), pRay.getDirection(), NumericTypeInfo<Float>::maxValue()), pVoxelGrid.getAABB(), rayAndAABBIntersectionData))
			{
				// If the ray does not intersect the AABB of the voxel grid, then the ray does not intersect any voxel in the voxel grid
				return false;
			}

			// Move the origin of the ray to the point on the AABB at which the ray enters the AABB
			pRay.setOrigin(pRay.getOrigin() + pRay.getDirection() * rayAndAABBIntersectionData.distance);

			// Calculate the integer coordinates of the voxel that the origin of the ray is in
			for(SizeType i = 0; i < 3; ++i)
			{
				// If the coord is the one for which the corresponding coord in the normal is 1.0f, then the coord will be 0.5f.
				// Just set the integer coordinate explicitly, in order to avoid floating point rounding error when rounding 0.5f to the nearest integer.
				// If the coord is not the one for which the normal is 1.0f, then it can be rounded to the nearest integer

				if(rayAndAABBIntersectionData.normal[i] > 0.5f)
				{
					currentVoxelCoords[i] = pVoxelGrid.getMaxVoxelCoords()[i];
				}
				else if(rayAndAABBIntersectionData.normal[i] < -0.5f)
				{
					currentVoxelCoords[i] = pVoxelGrid.getMinVoxelCoords()[i];
				}
				else
				{
					currentVoxelCoords[i] = roundFloatToInteger<VoxelGrid::VoxelCoordType>(pRay.getOrigin()[i]);
				}
			}
		}
		
		// The values used to increment the current voxel coordinates every time the ray exits a voxel boundary
		IntegerVector3<VoxelGrid::VoxelCoordType> voxelCoordIncrements;
		// Each component holds the distance that the ray must travel along its direction in order to increase its corresponding coordinate by 1.0f
		Vector3 rayDistancesBetweenCoords;
		// Each component holds the distance that the ray needs to travel for the whole number part of the corresponding coordinate to reach its next value
		Vector3 rayDistancesToNextCoords;

		// Initialize the iteration variables
		// TODO: Using a loop and indexing here instead of setting the x, y, and z components individually seems to perform a bit slower.
		//		 Is there any way to make the indexing faster?
		for(SizeType i = 0; i < 3; ++i)
		{
			if(pRay.getDirection()[i] > 0.0f)
			{
				voxelCoordIncrements[i] = 1;
				rayDistancesBetweenCoords[i] = 1.0f / pRay.getDirection()[i];
				rayDistancesToNextCoords[i] = (static_cast<Float>(currentVoxelCoords[i]) + 0.5f - pRay.getOrigin()[i]) * rayDistancesBetweenCoords[i];
			}
			else if(pRay.getDirection()[i] < 0.0f)
			{
				voxelCoordIncrements[i] = -1;
				rayDistancesBetweenCoords[i] = 1.0f / abs(pRay.getDirection()[i]);
				rayDistancesToNextCoords[i] = (pRay.getOrigin()[i] - (static_cast<Float>(currentVoxelCoords[i]) - 0.5f)) * rayDistancesBetweenCoords[i];
			}
			else
			{
				voxelCoordIncrements[i] = 0;
				rayDistancesBetweenCoords[i] = NumericTypeInfo<Float>::maxValue();
				rayDistancesToNextCoords[i] = NumericTypeInfo<Float>::maxValue();
			}
		}

		SizeType numberOfVoxelsTraversed = 0;
		pIntersectionData.wasNonEmptyVoxelHit = false;
		IntegerVector3<VoxelGrid::VoxelCoordType> previousVoxelCoords;

		// Iterate through the voxels that the ray intersects in order.
		// Each iteration of this loop processes one voxel and updates the iteration data to indicate the next voxel
		while(numberOfVoxelsTraversed < pMaximumNumberOfVoxelsToTraverse && pVoxelGrid.areCoordinatesWithinBounds(currentVoxelCoords))
		{
			// Get the data for the current voxel
			VoxelData currentVoxelData = pVoxelGrid.getVoxelData(currentVoxelCoords);

			// If the current voxel is not empty, then the value true must be returned from this function
			if(pShouldStopOnNonEmptyVoxelHit && !currentVoxelData.getIsEmpty())
			{
				pIntersectionData.wasNonEmptyVoxelHit = true;
				break;
			}

			// Record the current voxel's coords, in case the next one ends up being out of bounds
			previousVoxelCoords = currentVoxelCoords;

			// Update the increment variables
			if(rayDistancesToNextCoords.x < rayDistancesToNextCoords.y)
			{
				if(rayDistancesToNextCoords.x < rayDistancesToNextCoords.z)
				{
					if(rayDistancesToNextCoords.x > pRay.getLength())
					{
						break;
					}

					currentVoxelCoords.x += voxelCoordIncrements.x;
					rayDistancesToNextCoords.x += rayDistancesBetweenCoords.x;
				}
				else
				{
					if(rayDistancesToNextCoords.z > pRay.getLength())
					{
						break;
					}

					currentVoxelCoords.z += voxelCoordIncrements.z;
					rayDistancesToNextCoords.z += rayDistancesBetweenCoords.z;
				}
			}
			else
			{
				if(rayDistancesToNextCoords.y < rayDistancesToNextCoords.z)
				{
					if(rayDistancesToNextCoords.y > pRay.getLength())
					{
						break;
					}

					currentVoxelCoords.y += voxelCoordIncrements.y;
					rayDistancesToNextCoords.y += rayDistancesBetweenCoords.y;
				}
				else
				{
					if(rayDistancesToNextCoords.z > pRay.getLength())
					{
						break;
					}

					currentVoxelCoords.z += voxelCoordIncrements.z;
					rayDistancesToNextCoords.z += rayDistancesBetweenCoords.z;
				}
			}

			++numberOfVoxelsTraversed;
		}

		// If the current voxel is out of bounds, then we select the last voxel
		if(pVoxelGrid.areCoordinatesWithinBounds(currentVoxelCoords))
		{
			pIntersectionData.voxelCoords = currentVoxelCoords;
		}
		else
		{
			pIntersectionData.voxelCoords = previousVoxelCoords;
		}

		RayAndAABBIntersectionData rayAndVoxelAABBIntersectionData;
		// Do a ray and AABB test to get intersection data for the ray and the current voxel
		{
			AABB voxelAABB;
			voxelAABB.setMinPoint(Vector3(pIntersectionData.voxelCoords.x - 0.5f, pIntersectionData.voxelCoords.y - 0.5f, pIntersectionData.voxelCoords.z - 0.5f));
			voxelAABB.setMaxPoint(Vector3(pIntersectionData.voxelCoords.x + 0.5f, pIntersectionData.voxelCoords.y + 0.5f, pIntersectionData.voxelCoords.z + 0.5f));
			rayAndAABBIntersectionTest(Ray(pRay.getOrigin(), pRay.getDirection(), NumericTypeInfo<Float>::maxValue()), voxelAABB, rayAndVoxelAABBIntersectionData);
		}

		// Fill in the output intersection data
		{
			pIntersectionData.voxelData = pVoxelGrid.getVoxelData(pIntersectionData.voxelCoords);
			pIntersectionData.distance = rayAndVoxelAABBIntersectionData.distance;
			pIntersectionData.point = pRay.getOrigin() + pRay.getDirection() * rayAndVoxelAABBIntersectionData.distance;
			pIntersectionData.normal = rayAndVoxelAABBIntersectionData.normal;
		}

		// The ray intersects the voxel grid
		return true;
	}
}