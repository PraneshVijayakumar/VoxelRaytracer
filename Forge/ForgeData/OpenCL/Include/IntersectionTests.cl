#ifndef FORGE_CL_INTERSECTION_TESTS_CLH
#define FORGE_CL_INTERSECTION_TESTS_CLH

#include "../Forge/ForgeData/OpenCL/Include/Ray.cl"
#include "../Forge/ForgeData/OpenCL/Include/AABB.cl"
#include "../Forge/ForgeData/OpenCL/Include/Sphere.cl"

// Holds data about the intersection between a ray and an AABB
struct RayAndAABBIntersectionData
{
	// The distance from the ray's origin along the ray's direction that the ray enters the AABB
	float entryDistance;
	// The distance from the ray's origin along the ray's direction that the ray exits the AABB
	float exitDistance;
	// The normal of the face of the AABB that the ray enters the AABB through
	float3 normal;
};

bool rayAndAABBIntersectionTest(struct Ray pRay, struct AABB pAABB, struct RayAndAABBIntersectionData *pIntersectionData, float pMaxRayDistance)
{
	// The signed distance from the ray's origin along its direction at which it enters the AABB
	float rayInsideAABBRangeMin = 0.0f;
	// The signed distance from the ray's origin along its direction at which it exits the AABB
	float rayInsideAABBRangeMax = pMaxRayDistance;
	// The normal of the AABB face that the ray enters
	// TODO: Can the conversion of the vectors to arrays here be avoided?
	float currentRayAABBEntryFaceNormal[3];
	float rayOriginArray[3] = { pRay.origin.x, pRay.origin.y, pRay.origin.z };
	float rayDirectionArray[3] = { pRay.direction.x, pRay.direction.y, pRay.direction.z };
	float aabbMinPointArray[3] = { pAABB.minPoint.x, pAABB.minPoint.y, pAABB.minPoint.z };
	float aabbMaxPointArray[3] = { pAABB.maxPoint.x, pAABB.maxPoint.y, pAABB.maxPoint.z };

	// This loop processes each of the three coordinates x, y, and z
	for(int i = 0; i < 3; ++i)
	{
		// Test if the ray is parallel to the AABB plane.
		// This case needs to be handles first. If it is not, there might be a division by zero later
		// TODO: Can the test for pRay.direction[i] == 0.0f be done better/faster here?
		if(!(rayDirectionArray[i] > 0.0f || rayDirectionArray[i] < 0.0f))
		{
			// If the ray is parallel to the AABB plane and the origin is outside the AABB, then the ray cannot intersect the plane
			if(rayOriginArray[i] < aabbMinPointArray[i] || rayOriginArray[i] > aabbMaxPointArray[i])
			{
				return false;
			}
		}
		else
		{
			// The signed distance that the ray needs to travel starting from its origin in its direction in order to
			// increase the absolute value of the coordinate that this loop is processing by 1.0f
			float rayDistanceIncreaseForOneCoordIncrease = 1.0f / rayDirectionArray[i];

			// The signed distance from the ray origin in the ray's direction to the nearest point with the coordinate within range
			float rayInsideCoordRangeMin = (aabbMinPointArray[i] - rayOriginArray[i]) * rayDistanceIncreaseForOneCoordIncrease;
			// The signed distance from the ray origin in the ray's direction to the furthest point with the coordinate within range
			float rayInsideCoordRangeMax = (aabbMaxPointArray[i] - rayOriginArray[i]) * rayDistanceIncreaseForOneCoordIncrease;
			
			// The nearest and furthest points in the ray's direction will depend on the ray's direction in this coordinate.
			// Just swap the nearest and furthest points if the value of the nearest point is greater then the value of the furthest point
			if(rayInsideCoordRangeMin > rayInsideCoordRangeMax)
			{
				float rayInsideCoordRangeMinCopy = rayInsideCoordRangeMin;
				rayInsideCoordRangeMin = rayInsideCoordRangeMax;
				rayInsideCoordRangeMax = rayInsideCoordRangeMinCopy;
			}

			// Update the min value of the ray inside AABB range if needed
			if(rayInsideCoordRangeMin > rayInsideAABBRangeMin)
			{
				rayInsideAABBRangeMin = rayInsideCoordRangeMin;

				// Calculate the normal. If a new min value for the ray inside AABB range is obtained,
				// that means we have updated the face of the AABB through which the ray enters.
				// So the normal must be updated based on which face this is
				for(int i2 = 0; i2 < 3; ++i2)
				{
					if(i == i2)
					{
						if(rayDirectionArray[i2] > 0.0f)
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
	pIntersectionData->entryDistance = rayInsideAABBRangeMin;
	pIntersectionData->exitDistance = rayInsideAABBRangeMax;
	pIntersectionData->normal.x = currentRayAABBEntryFaceNormal[0];
	pIntersectionData->normal.y = currentRayAABBEntryFaceNormal[1];
	pIntersectionData->normal.z = currentRayAABBEntryFaceNormal[2];

	return true;
}

// Holds data about the intersection between a ray and a sphere
struct RayAndSphereIntersectionData
{
	float distance;
	float3 point;
	float3 normal;
};

bool rayAndSphereIntersectionTest(struct Ray pRay, struct Sphere pSphere, struct RayAndSphereIntersectionData *pRayAndSphereIntersectionData)
{
	float3 e = pSphere.center - pRay.origin;

	// Exit if the ray points away from the sphere
	if(dot(e, pRay.direction) < 0.0)
	{
		return false;
	}

	float a = dot(e, pRay.direction);

	float eSquare = length(e) * length(e);

	float rSquare = pSphere.radius * pSphere.radius;

	float aSquare = a * a;

	float fSquare = rSquare - eSquare + aSquare;

	// Determinant is negative, so the ray does not intersect the sphere
	if(fSquare < 0.0)
	{
		return false;
	}

	float intersectionInterval;

	// If the ray origin is inside the sphere, the interval will be negative, so cap it to 0.0f
	if(eSquare < rSquare)
	{
		intersectionInterval = 0.0;
	}
	else
	{
		intersectionInterval = a - sqrt(fSquare);
	}

	pRayAndSphereIntersectionData->distance = intersectionInterval;
	pRayAndSphereIntersectionData->point = pRay.origin + pRay.direction * intersectionInterval;
	pRayAndSphereIntersectionData->normal = normalize(pRayAndSphereIntersectionData->point - pSphere.center);
	return true;
}

#endif