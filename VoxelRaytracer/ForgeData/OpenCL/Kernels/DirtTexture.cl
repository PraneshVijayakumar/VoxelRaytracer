#include "../Forge/ForgeData/OpenCL/Include/Random.cl"
#include "ForgeData/OpenCL/Include/Texture.cl"

// This kernel gets executed for every voxel in the voxel grid. The global ID in the first, second and third dimensions correspond
// to the integer coordinates of the voxel the current work item is being executed for
__kernel void dirtTextureKernel(__global struct Texture *pTexturesBuffer,
								uint pTextureIndex) // An array in which each element represents the output data for a pixel
{
	// Get the coordinates of the texel for which this kernel instance is being executed for
	int2 texelCoords;
	texelCoords.x = get_global_id(0);
	texelCoords.y = get_global_id(1);

	// Get the size of the texture
	int2 textureSize;
	textureSize.x = get_global_size(0);
	textureSize.y = get_global_size(1);

	float2 texelCoordsFloat = convert_float2(texelCoords);

	float4 texelColor = (float4)(0.0f, 1.0f, 0.0f, 0.0f) * mix(0.2f, 0.4f, hash2DGrid(texelCoords));
	//float4 texelColor = (float4)(0.0f, 1.0f, 0.0f, 0.0f) * hash2DGrid(1, texelCoords);

	pTexturesBuffer[pTextureIndex].texels[texelCoords.x][texelCoords.y] = texelColor;

	/*pTexturesBuffer[pTextureIndex].texels[texelCoords.x][texelCoords.y].x = mix(0.0f, 1.0f, (float)texelCoords.x / (float)textureSize.x);
	pTexturesBuffer[pTextureIndex].texels[texelCoords.x][texelCoords.y].y = mix(0.0f, 1.0f, (float)texelCoords.y / (float)textureSize.y);
	pTexturesBuffer[pTextureIndex].texels[texelCoords.x][texelCoords.y].z = 0.0f;
	pTexturesBuffer[pTextureIndex].texels[texelCoords.x][texelCoords.y].w = 0.0f;*/
}