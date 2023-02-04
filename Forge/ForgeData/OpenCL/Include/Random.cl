#ifndef FORGE_CL_RANDOM_CLH
#define FORGE_CL_RANDOM_CLH

__constant uint COMMON_SEED = 5;

// Constants used for hashing
__constant uint prime32_2 = 0x85EBCA77U;  // 0b10000101111010111100101001110111
__constant uint prime32_3 = 0xC2B2AE3DU;  // 0b11000010101100101010111000111101
__constant uint prime32_4 = 0x27D4EB2FU;  // 0b00100111110101001110101100101111
__constant uint prime32_5 = 0x165667B1U;  // 0b00010110010101100110011110110001

uint rotateLeft(uint pValueToRotate, uint pAmountToRotateBy)
{
	return (pValueToRotate << pAmountToRotateBy) | (pValueToRotate >> (32 - pAmountToRotateBy));
}

uint hashAvalanche(uint pAccumulatedValue)
{
	// NOTE: Using a variation of the 32-bit xxHash algorithm for hashing

	uint result = pAccumulatedValue;
	result ^= result >> 15;
	result *= prime32_2;
	result ^= result >> 13;
	result *= prime32_3;
	result ^= result >> 16;

	return result;
}

uint initializeAccumulator(uint pSeed)
{
	return pSeed + prime32_5;
}

void hashAccumulate(uint *pAccumulator, uint pValueToAccumulate)
{
	(*pAccumulator) = rotateLeft((*pAccumulator) + pValueToAccumulate * prime32_3, 17) * prime32_4;
	(*pAccumulator) = hashAvalanche(*pAccumulator);
}

float randomFloat(uint *pHashAccumulator)
{
	float randomFloat;
	hashAccumulate(pHashAccumulator, *pHashAccumulator);
	randomFloat = (float)((*pHashAccumulator) & 0x0000ffff) / 65535.0f;

	return randomFloat;
}

float3 randomFloat3(uint *pHashAccumulator)
{
	float3 randomFloat3;
	hashAccumulate(pHashAccumulator, *pHashAccumulator);
	randomFloat3.x = (float)((*pHashAccumulator) & 0x0000ffff) / 65535.0f;
	hashAccumulate(pHashAccumulator, *pHashAccumulator);
	randomFloat3.y = (float)((*pHashAccumulator) & 0x0000ffff) / 65535.0f;
	hashAccumulate(pHashAccumulator, *pHashAccumulator);
	randomFloat3.z = (float)((*pHashAccumulator) & 0x0000ffff) / 65535.0f;

	return randomFloat3;
}

float3 randomInUnitSphere(uint *pHashAccumulator)
{
	return mix((float3)(-0.5f, -0.5f, -0.5f), (float3)(0.5f, 0.5f, 0.5f), randomFloat3(pHashAccumulator));
}

float hash2DGridWithSeed(uint pSeed, int2 pCoordinates)
{
	// NOTE: Using a variation of the 32-bit xxHash algorithm for hashing

	// Initialize the accumulator
	uint accumulator = initializeAccumulator(pSeed);

	// Eat first coordinate
	hashAccumulate(&accumulator, (uint)(pCoordinates.x + 2147482647));
	// Eat second coordinate
	hashAccumulate(&accumulator, (uint)(pCoordinates.y + 2147482647));

	return (float)(accumulator & 0x0000ffff) / 65535.0f;
}

float hash2DGrid(int2 pCoordinates)
{
	return hash2DGridWithSeed(COMMON_SEED, pCoordinates);
}

float hash3DGrid(const ulong pSeed, int3 pCoordinates)
{
	ulong hashState = pSeed;
	hashAccumulate(&hashState, (uint)(pCoordinates.x + 2147482647));
	hashAccumulate(&hashState, (uint)(pCoordinates.y + 2147482647));
	hashAccumulate(&hashState, (uint)(pCoordinates.z + 2147482647));
	return (float)(hashState & 0x000000000000ffff) / 65535.0f;
}

float valueNoise2DWithSeed(uint pSeed, float2 pCoordinates)
{
	float2 fractionalParts;
	float2 integerParts;
	fractionalParts = fract(pCoordinates, &integerParts);
	int2 bottomLeftIntegerCoords;
	bottomLeftIntegerCoords = convert_int2(integerParts);

	float2 lerpParams = fractionalParts * fractionalParts * (3.0f - 2.0f * fractionalParts);

	float bottomLerpedParam = mix(hash2DGridWithSeed(pSeed, bottomLeftIntegerCoords + (int2)(0, 0)),
							      hash2DGridWithSeed(pSeed, bottomLeftIntegerCoords + (int2)(1, 0)),
							      lerpParams.x);
	float topLerpedParam = mix(hash2DGridWithSeed(pSeed, bottomLeftIntegerCoords + (int2)(0, 1)),
							   hash2DGridWithSeed(pSeed, bottomLeftIntegerCoords + (int2)(1, 1)),
							   lerpParams.x);

	return mix(bottomLerpedParam, topLerpedParam, lerpParams.y);
}

float valueNoise2D(float2 pCoordinates)
{
	return valueNoise2DWithSeed(COMMON_SEED, pCoordinates);
}

float valueNoise3D(const ulong pSeed, float3 pCoordinates)
{
	float3 fractionalParts;
	float3 integerParts;
	fractionalParts = fract(pCoordinates, &integerParts);
	int3 bottomLeftIntegerCoords;
	bottomLeftIntegerCoords = convert_int3(integerParts);

	float3 lerpParams = fractionalParts * fractionalParts * (3.0f - 2.0f * fractionalParts);

	float bottomBackLerpedParam = mix(hash3DGrid(pSeed, bottomLeftIntegerCoords + (int3)(0, 0, 0)),
									  hash3DGrid(pSeed, bottomLeftIntegerCoords + (int3)(1, 0, 0)),
									  lerpParams.x);
	float topBackLerpedParam = mix(hash3DGrid(pSeed, bottomLeftIntegerCoords + (int3)(0, 1, 0)),
								   hash3DGrid(pSeed, bottomLeftIntegerCoords + (int3)(1, 1, 0)),
								   lerpParams.x);
	float bottomFrontLerpedParam = mix(hash3DGrid(pSeed, bottomLeftIntegerCoords + (int3)(0, 0, 1)),
									   hash3DGrid(pSeed, bottomLeftIntegerCoords + (int3)(1, 0, 1)),
									   lerpParams.x);
	float topFrontLerpedParam = mix(hash3DGrid(pSeed, bottomLeftIntegerCoords + (int3)(0, 1, 1)),
								    hash3DGrid(pSeed, bottomLeftIntegerCoords + (int3)(1, 1, 1)),
								    lerpParams.x);
	float backLerpedParam = mix(bottomBackLerpedParam, topBackLerpedParam, lerpParams.y);
	float frontLerpedParam = mix(bottomFrontLerpedParam, topFrontLerpedParam, lerpParams.y);

	return mix(backLerpedParam, frontLerpedParam, lerpParams.z);
}

#endif