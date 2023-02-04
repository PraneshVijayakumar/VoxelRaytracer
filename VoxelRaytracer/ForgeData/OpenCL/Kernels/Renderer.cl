#include "../Forge/ForgeData/OpenCL/Include/Ray.cl"
#include "../Forge/ForgeData/OpenCL/Include/Sphere.cl"
#include "../Forge/ForgeData/OpenCL/Include/AABB.cl"
#include "../Forge/ForgeData/OpenCL/Include/Random.cl"
#include "../Forge/ForgeData/OpenCL/Include/IntersectionTests.cl"
#include "../Forge/ForgeData/OpenCL/Include/Utility.cl"
#include "ForgeData/OpenCL/Include/Voxel.cl"
#include "ForgeData/OpenCL/Include/ProgramIntersectionTests.cl"
#include "ForgeData/OpenCL/Include/Texture.cl"

struct RayHitData
{
	float3 color;
	float3 point;
	float3 normal;
};

bool traceRay(struct Ray pRay,
			  __global struct VoxelData *pVoxelOctreeNodesArray,
			  uint pVoxelOctreeDepth,
			  long3 pVoxelGridMinCoords,
			  long3 pVoxelGridMaxCoords,
			  float pMaxDistance,
			  int pIsOctreeVisualizationEnabled,
			  __global struct Texture *pTexturesBuffer,
			  struct RayHitData *pRayHitData)
{
	struct RayAndVoxelOctreeIntersectionData rayAndVoxelOctreeIntersectionData;
	if(rayAndVoxelOctreeIntersectionTest(pRay, pVoxelOctreeNodesArray, pVoxelOctreeDepth, pVoxelGridMinCoords, pVoxelGridMaxCoords, pMaxDistance, pIsOctreeVisualizationEnabled, &rayAndVoxelOctreeIntersectionData))
	{
		// The number of coordinates in the intersection point that have values that make them potentially part of a voxel's border
		int numberOfCoordinatesInBorderRange = 0;
		float3 rayHitPointFractionalPart = rayAndVoxelOctreeIntersectionData.point - floor(rayAndVoxelOctreeIntersectionData.point);
		if(rayHitPointFractionalPart.x > 0.4f && rayHitPointFractionalPart.x < 0.6f)
		{
			++numberOfCoordinatesInBorderRange;
		}
		if(rayHitPointFractionalPart.y > 0.4f && rayHitPointFractionalPart.y < 0.6f)
		{
			++numberOfCoordinatesInBorderRange;
		}
		if(rayHitPointFractionalPart.z > 0.4f && rayHitPointFractionalPart.z < 0.6f)
		{
			++numberOfCoordinatesInBorderRange;
		}

		// The fractional part of one of the coordinates of the intersection point will be 0.5f, because of the the way the voxels are placed in the world.
		// So at least one of the other coordinates must be in the border range for the intersection point to be a part of a voxel's border
		{
			if(numberOfCoordinatesInBorderRange > 1)
			{
				pRayHitData->color.x = 0;
				pRayHitData->color.y = 0;
				pRayHitData->color.z = 0;
			}
			else
			{
				// Calculate the texel coordinates from the uv coordinates
				uint2 texelCoordinates;
				texelCoordinates = convert_uint2(floor(rayAndVoxelOctreeIntersectionData.uvCoords * 15.0f));
				texelCoordinates = clamp(texelCoordinates, (uint2)(0, 0), (uint2)(15, 15));
					
				pRayHitData->color.x = pTexturesBuffer[0].texels[texelCoordinates.x][texelCoordinates.y].x;
				pRayHitData->color.y = pTexturesBuffer[0].texels[texelCoordinates.x][texelCoordinates.y].y;
				pRayHitData->color.z = pTexturesBuffer[0].texels[texelCoordinates.x][texelCoordinates.y].z;
			}
		}
		
		pRayHitData->point = rayAndVoxelOctreeIntersectionData.point;
		pRayHitData->normal = rayAndVoxelOctreeIntersectionData.normal;
		return true;
	}
	else
	{
		return false;
	}
}

float3 getBackgroundColorForRay(struct Ray pRay)
{
	float3 horizonColor = (float3)(170.0f / 255.0f, 209.0f / 255.0f, 254.0f / 255.0f);
	float3 skyColor = (float3)(120.0f / 255.0f, 167.0f / 255.0f, 255.0f / 255.0f);

	// TODO: Tried different easing functions here. Is there a better one that can be used?
	//float mixParam = 1.0f - pow(2, -10.0f * fabs(cameraRayForPixel.direction.y));
	//float mixParam = 1.0f - pow(1 - fabs(cameraRayForPixel.direction.y), 5.0f);
	//float mixParam = 1.0f - pow(1 - fabs(cameraRayForPixel.direction.y), 3.0f);
	float mixParam = 1.0f - (1.0f - fabs(pRay.direction.y)) * (1.0f - fabs(pRay.direction.y));

	return mix(horizonColor, skyColor, mixParam);
}

struct DebugAABB
{
	struct AABB aabb;
	float3 color;
};

float3 calculatePixelColor(int2 pPixelCoords,
						   int2 pOutputImageResolution,
						   float3 pCameraPosition, // The position of the camera in world space
						   float3 pCameraOrientationXAxis, // The x axis basis vector of the camera orientation
						   float3 pCameraOrientationYAxis, // The y axis basis vector of the camera orientation
						   float3 pCameraOrientationZAxis, // The z axis basis vector of the camera orientation
						   float pCameraVerticalFieldOfViewInDegrees, // The vertical field of view of the camera, specified in degrees
						   __global struct Voxel *pVoxelOctreeNodesArray,
						   uint pVoxelOctreeDepth,
						   __global struct Texture *pTexturesBuffer,
						   __global struct DebugAABB *pDebugAABBsArray,
						   int pDebugAABBArrayLength,
						   int pIsOctreeVisualizationEnabled)
{
	// Get the camera ray for the pixel
	struct Ray cameraRayForPixel = getCameraRayForPixel(pCameraPosition,
														pCameraOrientationXAxis,
														pCameraOrientationYAxis,
														pCameraOrientationZAxis,
														pCameraVerticalFieldOfViewInDegrees,
														pPixelCoords,
														pOutputImageResolution);

	for(int i = 0; i < pDebugAABBArrayLength; ++i)
	{
		// Create an AABB that represents the current single voxel edit coords
		struct AABB singleEditVoxelAABB;
		singleEditVoxelAABB.minPoint = pDebugAABBsArray[i].aabb.minPoint;
		singleEditVoxelAABB.maxPoint = pDebugAABBsArray[i].aabb.maxPoint;
		struct RayAndAABBIntersectionData rayAndSingleEditVoxelAABBIntersectionData;
		if(rayAndAABBIntersectionTest(cameraRayForPixel, singleEditVoxelAABB, &rayAndSingleEditVoxelAABBIntersectionData, MAXFLOAT))
		{
			int numberOfCoordinatesInBorderRange = 0;
			float3 rayHitPoint = cameraRayForPixel.origin + cameraRayForPixel.direction * rayAndSingleEditVoxelAABBIntersectionData.entryDistance;
			float3 rayHitPointFractionalPart = rayHitPoint - floor(rayHitPoint);
			if(rayHitPointFractionalPart.x > 0.4f && rayHitPointFractionalPart.x < 0.6f)
			{
				++numberOfCoordinatesInBorderRange;
			}
			if(rayHitPointFractionalPart.y > 0.4f && rayHitPointFractionalPart.y < 0.6f)
			{
				++numberOfCoordinatesInBorderRange;
			}
			if(rayHitPointFractionalPart.z > 0.4f && rayHitPointFractionalPart.z < 0.6f)
			{
				++numberOfCoordinatesInBorderRange;
			}

			// The fractional part of one of the coordinates of the intersection point will be 0.5f, because of the the way the voxels are placed in the world.
			// So at least one of the other coordinates must be in the border range for the intersection point to be a part of a voxel's border
			if(numberOfCoordinatesInBorderRange > 1)
			{
				// Give the pixel the debug AABB color
				return pDebugAABBsArray[i].color;
			}
		}
	}

	int octreeRootNodeScale = 2;
	for(uint i = 1; i < pVoxelOctreeDepth - 1; ++i)
	{
		octreeRootNodeScale *= 2;
	}
	long octreeRootNodeHalfScale = octreeRootNodeScale / 2.0f;
	long3 voxelGridMinCoords;
	voxelGridMinCoords.x = -octreeRootNodeHalfScale;
	voxelGridMinCoords.y = -octreeRootNodeHalfScale;
	voxelGridMinCoords.z = -octreeRootNodeHalfScale;
	long3 voxelGridMaxCoords;
	voxelGridMaxCoords.x = octreeRootNodeHalfScale - 1;
	voxelGridMaxCoords.y = octreeRootNodeHalfScale - 1;
	voxelGridMaxCoords.z = octreeRootNodeHalfScale - 1;
	float octreeRootNodeFloatHalfScale = (float)octreeRootNodeHalfScale;

	struct RayHitData rayHitData;
	if(traceRay(cameraRayForPixel, pVoxelOctreeNodesArray, pVoxelOctreeDepth, voxelGridMinCoords, voxelGridMaxCoords, FLT_MAX, pIsOctreeVisualizationEnabled, pTexturesBuffer, &rayHitData))
	{
		return rayHitData.color;
	}
	else
	{
		return getBackgroundColorForRay(cameraRayForPixel);
	}
}

// This kernel gets executed for every pixel in the backbuffer. The global ID in the first and second dimensions correspond
// to the integer coordinates of the pixel the current work item is being executed for
__kernel void renderPixel(float3 pCameraPosition, // The position of the camera in world space
						  float3 pCameraOrientationXAxis, // The x axis basis vector of the camera orientation
						  float3 pCameraOrientationYAxis, // The y axis basis vector of the camera orientation
						  float3 pCameraOrientationZAxis, // The z axis basis vector of the camera orientation
						  float pCameraVerticalFieldOfViewInDegrees, // The vertical field of view of the camera, specified in degrees
						  __global struct Voxel *pVoxelOctreeNodesArray,
						  uint pVoxelOctreeDepth,
						  __global struct Texture *pTexturesBuffer,
						  __global struct DebugAABB *pDebugAABBsArray,
						  int pDebugAABBArrayLength,
						  int pIsOctreeVisualizationEnabled,
						  __global uchar4 *pOutputUchar4Framebuffer) // An array in which each element represents the output data for a pixel
{
	// Get the coordinates of the pixel for which this kernel instance is being executed for
	int2 pixelCoords;
	pixelCoords.x = get_global_id(0);
	pixelCoords.y = get_global_id(1);

	// Get the resolution of the output image
	int2 outputImageResolution;
	outputImageResolution.x = get_global_size(0);
	outputImageResolution.y = get_global_size(1);

	// Calculate the index of the pixel data for this pixel
	int pixelIndex = outputImageResolution.x * pixelCoords.y + pixelCoords.x;

	float3 pixelColor = calculatePixelColor(pixelCoords,
											outputImageResolution,
											pCameraPosition,
											pCameraOrientationXAxis,
											pCameraOrientationYAxis,
											pCameraOrientationZAxis,
											pCameraVerticalFieldOfViewInDegrees,
											pVoxelOctreeNodesArray,
											pVoxelOctreeDepth,
											pTexturesBuffer,
											pDebugAABBsArray,
											pDebugAABBArrayLength,
											pIsOctreeVisualizationEnabled);

	// Apply gamma correction
	{
		float gammaCorrectionExponent = 1.0f / 2.2f;
		pixelColor = pow(pixelColor, (float3)(gammaCorrectionExponent, gammaCorrectionExponent, gammaCorrectionExponent));
	}

	// Convert the RGB pixel color to uchar and set the pixel color in the uchar4 framebuffer
	// NOTE: Red and Blue are flipped here because this uchar4 output buffer is meant to be used in a Windows framebuffer
	{
		pOutputUchar4Framebuffer[pixelIndex].x = (uchar)(255.0f * pixelColor.z);
		pOutputUchar4Framebuffer[pixelIndex].y = (uchar)(255.0f * pixelColor.y);
		pOutputUchar4Framebuffer[pixelIndex].z = (uchar)(255.0f * pixelColor.x);
		pOutputUchar4Framebuffer[pixelIndex].w = 0;
	}
}