#ifndef FORGE_INTERSECTION_TESTS_H
#define FORGE_INTERSECTION_TESTS_H

#include "Ray.h"
#include "AABB.h"
#include "Plane.h"
#include "Vector2.h"

namespace forge
{
	// Holds data about the intersection between a ray and an AABB
	struct RayAndAABBIntersectionData
	{
		// The distance from the ray's origin along the ray's direction that the ray enters the AABB
		Float distance;
		// The normal of the face of the AABB that the ray enters the AABB through
		Vector3 normal;
	};

	/// <summary>
	/// Perform an intersection test netween a ray and an AABB.
	/// TODO: This has not been tested after some changes were made to it
	/// </summary>
	/// <param name="pRay">The ray</param>
	/// <param name="pAABB">The AABB</param>
	/// <param name="pIntersectionData">The output parameter into which the intersection data will be written if an intersection is found</param>
	/// <returns>True if the ray and AABB are intersecting, false if they are not</returns>
	bool rayAndAABBIntersectionTest(const Ray &pRay, const AABB &pAABB, RayAndAABBIntersectionData &pIntersectionData);

	struct RayAndPlaneIntersectionData
	{
		Float distance;
		Vector3 normal;
		Vector3 point;
	};

	bool rayAndPlaneIntersectionTest(const Ray &pRay, const Plane &pPlane, RayAndPlaneIntersectionData *pRayAndPlaneIntersectionData);
}

#endif