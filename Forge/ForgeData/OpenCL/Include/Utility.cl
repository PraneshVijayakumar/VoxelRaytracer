#ifndef FORGE_CL_UTILITY_CLH
#define FORGE_CL_UTILITY_CLH

#include "../Forge/ForgeData/OpenCL/Include/Ray.cl"

// Get the camera ray for a pixel
struct Ray getCameraRayForPixel(float3 pCameraPosition, // The position of the camera in world space
								float3 pCameraOrientationXAxis, // The x axis basis vector of the camera orientation
								float3 pCameraOrientationYAxis, // The y axis basis vector of the camera orientation
								float3 pCameraOrientationZAxis, // The z axis basis vector of the camera orientation
								float pCameraVerticalFieldOfViewInDegrees, // The vertical field of view of the camera, specified in degrees
								int2 pPixelCoords, // The coordinates of the pixel for which to get the camera ray
								int2 pImageResolution) // The resolution of the image		
{
	struct Ray cameraRay;

	// The origin of the ray is the position of the camera
	cameraRay.origin = pCameraPosition;

	const float verticalViewExtent = tan(radians(pCameraVerticalFieldOfViewInDegrees / 2.0f));
	const float heightToWidthMultiplier = (float)(pImageResolution.x) / (float)(pImageResolution.y);
	const float horizontalViewExtent = verticalViewExtent * heightToWidthMultiplier;

	// Calculate the direction of the ray in camera space
	cameraRay.direction.x = mix(-horizontalViewExtent, horizontalViewExtent, (float)(pPixelCoords.x) / (float)(pImageResolution.x - 1));
	cameraRay.direction.y = mix(-verticalViewExtent, verticalViewExtent, (float)(pPixelCoords.y) / (float)(pImageResolution.y - 1));
	cameraRay.direction.z = 1.0f;

	// Convert the direction to world space
	cameraRay.direction = pCameraOrientationXAxis * cameraRay.direction.x +
						  pCameraOrientationYAxis * cameraRay.direction.y +
						  pCameraOrientationZAxis * cameraRay.direction.z;

	// Normalize the direction
	cameraRay.direction = normalize(cameraRay.direction);

	return cameraRay;
}

#endif