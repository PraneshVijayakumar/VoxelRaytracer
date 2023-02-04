#include "OpenCLKernel.h"
#include <Forge/Source/Core/Containers/ResizableArray.h>
#include <Forge/Source/Core/Utility.h>
#include <filesystem>

namespace forge
{
	void OpenCLKernel::createProgramWithSourceString(OpenCLManager *pOpenCLManager, const char *pSourceTextString)
	{
		// Used to store the return code from OpenCL functions
		cl_int returnCode;

		// Create the program
		const char *kernelCStringPointer = pSourceTextString;
		const size_t kernelStringLength = getCharacterArrayLength(pSourceTextString);
		this->openCLProgram = clCreateProgramWithSource(pOpenCLManager->getContext(), 1, &kernelCStringPointer, &kernelStringLength, &returnCode);
		FORGE_OPENCL_ERROR_CODE_CHECK(returnCode);

		// Build the program
		cl_device_id deviceID = pOpenCLManager->getDeviceID();
		
		std::filesystem::path currentDirectory = std::filesystem::current_path();
		std::string programBuildOptionsString = "-I " + currentDirectory.string();
		returnCode = clBuildProgram(this->openCLProgram, 1, &deviceID, programBuildOptionsString.c_str(), NULL, NULL);
		FORGE_OPENCL_ERROR_CODE_CHECK(returnCode);

		// Check if the program build succeded
		cl_build_status buildStatus;
		clGetProgramBuildInfo(this->openCLProgram, deviceID, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &buildStatus, NULL);
		if(buildStatus != CL_BUILD_SUCCESS)
		{
			// Print the error log if the program build failed
			size_t buildLogSize = 0;
			clGetProgramBuildInfo(this->openCLProgram, deviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &buildLogSize);
			if(buildLogSize > 0)
			{
				ResizableArray<char> buildLog(buildLogSize);
				clGetProgramBuildInfo(this->openCLProgram, deviceID, CL_PROGRAM_BUILD_LOG, buildLogSize, &buildLog[0], &buildLogSize);
				FORGE_OPENCL_ERROR("Program build failure : ", &buildLog[0]);
			}
		}
	}
}