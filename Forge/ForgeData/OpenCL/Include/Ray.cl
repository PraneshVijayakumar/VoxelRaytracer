#ifndef FORGE_CL_RAY_CLH
#define FORGE_CL_RAY_CLH

// Represents a ray of the maximum possible length
struct Ray
{
	float3 origin;
	// The direction must be normalized
	float3 direction;
};

#endif