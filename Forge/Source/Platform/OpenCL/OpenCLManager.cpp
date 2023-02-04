#include "OpenCLManager.h"
#include <Forge/Source/Platform/OpenCL/OpenCLLogging.h>
#include <Forge/Source/Platform/OpenCL/OpenCLUtility.h>
#include <Forge/Source/Core/Containers/ResizableArray.h>

namespace forge
{
	void OpenCLManager::startup()
	{
		// Used to store the return code from various OpenCL functions
		cl_int returnCode;

		// Get the IDs for the available platforms
		cl_uint numberOfPlatforms;
		FORGE_OPENCL_ERROR_CODE_CHECK(clGetPlatformIDs(0, NULL, &numberOfPlatforms));
		ResizableArray<cl_platform_id> platformIDs(numberOfPlatforms);
		FORGE_OPENCL_ERROR_CODE_CHECK(clGetPlatformIDs(numberOfPlatforms, &platformIDs[0], NULL));

		bool suitableDeviceFound = false;
		cl_platform_id bestDevicePlatformID = cl_platform_id();
		cl_device_id bestDeviceID = cl_device_id();

		// For each of available platforms
		for(SizeType i = 0; i < platformIDs.getNumberOfElements(); ++i)
		{
			// Get all the devices available on the platform
			cl_uint numberOfDevices;
			FORGE_OPENCL_ERROR_CODE_CHECK(clGetDeviceIDs(platformIDs[i], CL_DEVICE_TYPE_ALL, 0, NULL, &numberOfDevices));
			ResizableArray<cl_device_id> deviceIDs(numberOfDevices);
			FORGE_OPENCL_ERROR_CODE_CHECK(clGetDeviceIDs(platformIDs[i], CL_DEVICE_TYPE_ALL, numberOfDevices, &deviceIDs[0], NULL));

			// For each of the devices available on the platform
			for(SizeType i2 = 0; i2 < deviceIDs.getNumberOfElements(); ++i2)
			{
				// Get the amount of global device memory available on the device
				size_t deviceTypeValueSize;
				FORGE_OPENCL_ERROR_CODE_CHECK(clGetDeviceInfo(deviceIDs[i2], CL_DEVICE_TYPE, 0, NULL, &deviceTypeValueSize));
				cl_device_type deviceType;
				FORGE_OPENCL_ERROR_CODE_CHECK(clGetDeviceInfo(deviceIDs[i2], CL_DEVICE_TYPE, deviceTypeValueSize, &deviceType, NULL));

				// NOTE: Choosing the first GPU
				if(deviceType == CL_DEVICE_TYPE_GPU)
				{
					bestDevicePlatformID = platformIDs[i];
					bestDeviceID = deviceIDs[i2];
					suitableDeviceFound = true;
					break;
				}
			}

			if (suitableDeviceFound)
			{
				break;
			}
		}

		if(suitableDeviceFound)
		{
			this->platformID = bestDevicePlatformID;
			this->deviceID = bestDeviceID;

			// Debug log the names of the selected platform and device
			{
				// Get the name of the platform
				size_t platformNameSize;
				FORGE_OPENCL_ERROR_CODE_CHECK(clGetPlatformInfo(bestDevicePlatformID, CL_PLATFORM_NAME, 0, NULL, &platformNameSize));
				ResizableArray<char> platformName(platformNameSize);
				FORGE_OPENCL_ERROR_CODE_CHECK(clGetPlatformInfo(bestDevicePlatformID, CL_PLATFORM_NAME, platformNameSize, &platformName[0], NULL));

				// Log the name of the platform
				FORGE_DEBUG_LOG("Selected OpenCL platform : ", &platformName[0]);

				// Get the name of the device
				size_t deviceNameSize;
				FORGE_OPENCL_ERROR_CODE_CHECK(clGetDeviceInfo(bestDeviceID, CL_DEVICE_NAME, 0, NULL, &deviceNameSize));
				ResizableArray<char> devicename(deviceNameSize);
				FORGE_OPENCL_ERROR_CODE_CHECK(clGetDeviceInfo(bestDeviceID, CL_DEVICE_NAME, deviceNameSize, &devicename[0], NULL));

				// Log the name of the device
				FORGE_DEBUG_LOG("Selected OpenCL device : ", &devicename[0]);
			}

			// Create the context
			this->context = clCreateContext(NULL, 1, &bestDeviceID, NULL, NULL, &returnCode);
			FORGE_OPENCL_ERROR_CODE_CHECK(returnCode);

			// Create the command queue
			this->commandQueue = clCreateCommandQueue(this->context, bestDeviceID, 0, &returnCode);
			FORGE_OPENCL_ERROR_CODE_CHECK(returnCode);
		}
		else
		{
			FORGE_OPENCL_ERROR("OpenCLManager could not find a suitable device");
		}
	}

	void OpenCLManager::shutdown()
	{
		FORGE_OPENCL_ERROR_CODE_CHECK(clReleaseCommandQueue(this->commandQueue));
		FORGE_OPENCL_ERROR_CODE_CHECK(clReleaseContext(this->context));
	}
}