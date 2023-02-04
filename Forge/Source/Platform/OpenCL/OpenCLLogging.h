#ifndef FORGE_OPENCL_LOGGING_H
#define FORGE_OPENCL_LOGGING_H

#include <CL/cl.h>
#include <Forge/Source/Core/Logging.h>

#define FORGE_OPENCL_ERROR(...) FORGE_ERROR_LOG("OpenCL error : ", __VA_ARGS__)

#endif