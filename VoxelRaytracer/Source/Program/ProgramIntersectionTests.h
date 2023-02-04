#ifndef FORGE_PROGRAM_INTERSECTION_TESTS_H
#define FORGE_PROGRAM_INTERSECTION_TESTS_H

#include <Forge/Source/Core/Math/IntersectionTests.h>
#include <VoxelRaytracer/Source/Program/VoxelGrid.h>

namespace forge
{
	// Holds data about the intersection between a ray and a voxel grid
	struct RayAndVoxelDataIntersectionData
	{
		bool wasNonEmptyVoxelHit;
		// The coordinates of the first voxel that the ray is intersecting
		IntegerVector3<VoxelGrid::VoxelCoordType> voxelCoords;
		// The data of the first voxel that the ray is intersecting
		VoxelData voxelData;
		// The distance from the ray's origin along the ray's direction that the ray enters the voxel
		Float distance;
		// The point at which the ray enters the voxel
		Vector3 point;
		// The normal of the face of the voxel that the ray enters the voxel through
		Vector3 normal;
	};

	/// <summary>
	/// Perform an intersection test between a ray and a voxel grid.
	/// If they are intersecting, the data about the intersection is placed inside the intersection data.
	/// If the origin of the ray is outside the bounds of the voxel grid, it is clamped so that it intersects the bounds of the voxel grid.
	/// The ray length is maximum distance that the ray is allowed to travel from its origin in its direction before intersecting a voxel in the grid.
	///	This distance is counted after the ray origin is clamped to the bounds of the voxel grid, if the given origin is outside the bounds of the voxel grid
	/// </summary>
	/// <param name="pRay">The ray</param>
	/// <param name="pVoxelData">The voxel grid</param>
	/// <param name="pIntersectionData">The output parameter into which the result of the intersection test will be written if an intersection is found</param>
	/// <param name="pShouldStopOnNonEmptyVoxelHit">Whether the ray iteration should stop once the ray hits a non empty voxel.
	///				 The non empty voxel's data will be returned in the intersection data</param>
	/// <param name="pMaximumNumberOfVoxelsToTraverse">The maximum number of voxels, excluding the starting voxel, that the ray is allowed to traverse</param>
	/// <returns>True if the ray intersects the given voxel grid AABB, false if it does not</returns>
	bool rayAndVoxelGridIntersectionTest(Ray pRay,
										 const VoxelGrid &pVoxelGrid,
										 RayAndVoxelDataIntersectionData &pIntersectionData,
										 bool pShouldStopOnNonEmptyVoxelHit = true,
										 SizeType pMaximumNumberOfVoxelsToTraverse = NumericTypeInfo<SizeType>::maxValue());
}

#endif