#include "IntersectionTests.h"
#include <Forge/Source/Core/NumericTypeInfo.h>
#include <Forge/Source/Core/Math/MathUtility.h>
#include <Forge/Source/Core/Utility.h>

namespace forge
{
	bool rayAndAABBIntersectionTest(const Ray &pRay, const AABB &pAABB, RayAndAABBIntersectionData &pIntersectionData)
	{
		// The signed distance from the ray's origin along its direction at which it enters the AABB
		Float rayInsideAABBRangeMin = 0.0f;
		// The signed distance from the ray's origin along its direction at which it exits the AABB
		Float rayInsideAABBRangeMax = pRay.getLength();
		// The normal of the AABB face that the ray enters
		Vector3 currentRayAABBEntryFaceNormal;

		// This loop processes each of the three coordinates x, y, and z
		for(SizeType i = 0; i < 3; ++i)
		{
			// Test if the ray is parallel to the AABB plane.
			// This case needs to be handled first. If it is not, there might be a division by zero later.
			// TODO: Can the test for pRay.direction[i] == 0.0f be done better here?
			if(abs<Float>(pRay.getDirection()[i]) < NumericTypeInfo<Float>::epsilon())
			{
				// If the ray is parallel to the AABB plane and the origin is outside the AABB, then the ray cannot intersect the plane
				if(pRay.getOrigin()[i] < pAABB.getMinPoint()[i] || pRay.getOrigin()[i] > pAABB.getMaxPoint()[i])
				{
					return false;
				}
			}
			else
			{
				// The signed distance that the ray needs to travel starting from its origin in its direction in order to
				// increase the absolute value of the coordinate that this loop is processing by 1.0f
				Float rayDistanceIncreaseForOneCoordIncrease = 1.0f / pRay.getDirection()[i];

				// The signed distance from the ray origin in the ray's direction to the nearest point with the coordinate within range
				Float rayInsideCoordRangeMin = (pAABB.getMinPoint()[i] - pRay.getOrigin()[i]) * rayDistanceIncreaseForOneCoordIncrease;
				// The signed distance from the ray origin in the ray's direction to the furthest point with the coordinate within range
				Float rayInsideCoordRangeMax = (pAABB.getMaxPoint()[i] - pRay.getOrigin()[i]) * rayDistanceIncreaseForOneCoordIncrease;
				
				// The nearest and furthest points in the ray's direction will depend on the ray's direction in this coordinate.
				// Just swap the nearest and furthest points if the value of the nearest point is greater then the value of the furthest point
				if(rayInsideCoordRangeMin > rayInsideCoordRangeMax)
				{
					swap(rayInsideCoordRangeMin, rayInsideCoordRangeMax);
				}

				// Update the min value of the ray inside AABB range if needed
				if(rayInsideCoordRangeMin > rayInsideAABBRangeMin)
				{
					rayInsideAABBRangeMin = rayInsideCoordRangeMin;

					// Calculate the normal. If a new min value for the ray inside AABB range is obtained,
					// that means we have updated the face of the AABB through which the ray enters.
					// So the normal must be updated based on which face this is
					for(SizeType i2 = 0; i2 < 3; ++i2)
					{
						if(i == i2)
						{
							if(pRay.getDirection()[i2] > 0.0f)
							{
								currentRayAABBEntryFaceNormal[i2] = -1.0f;
							}
							else
							{
								currentRayAABBEntryFaceNormal[i2] = 1.0f;
							}
						}
						else
						{
							currentRayAABBEntryFaceNormal[i2] = 0.0f;
						}
					}
				}
				// Update the max value of the ray inside AABB range if needed
				if(rayInsideCoordRangeMax < rayInsideAABBRangeMax)
				{
					rayInsideAABBRangeMax = rayInsideCoordRangeMax;
				}

				// If the ray inside AABB range becomes less than zero, then the ray does not intersect the AABB
				if(rayInsideAABBRangeMin > rayInsideAABBRangeMax)
				{
					return false;
				}
			}
		}

		// The ray inside AABB range is greater than or equal to zero, so the ray intersects the AABB
		pIntersectionData.distance = rayInsideAABBRangeMin;
		pIntersectionData.normal = currentRayAABBEntryFaceNormal;

		return true;
	}

	bool rayAndPlaneIntersectionTest(const Ray &pRay, const Plane &pPlane, RayAndPlaneIntersectionData *pRayAndPlaneIntersectionData)
	{
		Float denominator = Vector3::dot(pRay.getDirection(), pPlane.getNormal());

		// If the ray is parallel to the plane, then the value of the denomoinator will be 0.0f.
		// If the ray is facing the same direction as the plane, the denominator will be greater than 0.0f.
		// There must be no intersection in the first case
		if(denominator == 0.0f)
		{
			return false;
		}

		// The ray intersects the plane, so calculate the output data and return true
		pRayAndPlaneIntersectionData->distance = (pPlane.getDistance() - Vector3::dot(pRay.getOrigin(), pPlane.getNormal())) / denominator;
		pRayAndPlaneIntersectionData->normal = pPlane.getNormal();
		pRayAndPlaneIntersectionData->point = pRay.getOrigin() + pRay.getDirection() * pRayAndPlaneIntersectionData->distance;

		if(pRayAndPlaneIntersectionData->distance > 0.0f && pRayAndPlaneIntersectionData->distance < pRay.getLength())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}