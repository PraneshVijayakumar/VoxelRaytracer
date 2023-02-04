#ifndef FORGE_OPENCL_UTILITY_H
#define FORGE_OPENCL_UTILITY_H

#include <CL/cl.h>
#include <Forge/Source/Core/Math/Vector3.h>
#include "OpenCLLogging.h"

#define FORGE_OPENCL_ERROR_CODE_CHECK(pReturnValue) \
do \
{ \
	if(pReturnValue != CL_SUCCESS) \
	{ \
		FORGE_OPENCL_ERROR("The value of " #pReturnValue " indicates an error"); \
	} \
} \
while(false)

namespace forge
{
	inline
	cl_float3 convertVector3Tocl_float3(const Vector3 pVector3)
	{
		cl_float3 conversionResult;
		conversionResult.x = pVector3.x;
		conversionResult.y = pVector3.y;
		conversionResult.z = pVector3.z;
		
		return conversionResult;
	}
}

#endif