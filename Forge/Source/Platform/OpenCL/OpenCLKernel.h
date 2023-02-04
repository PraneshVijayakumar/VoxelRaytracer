#ifndef FORGE_OPENCL_KERNEL_H
#define FORGE_OPENCL_KERNEL_H

#include <CL/cl.h>
#include <Forge/Source/Platform/OpenCL/OpenCLUtility.h>
#include <Forge/Source/Core/Math/Vector2.h>
#include <Forge/Source/Core/Math/Vector3.h>
#include <Forge/Source/Core/Math/IntegerVector2.h>
#include <Forge/Source/Core/Math/IntegerVector3.h>
#include <Forge/Source/Platform/Windows/FileEditors/TextFileEditor.h>
#include "OpenCLManager.h"

namespace forge
{
	class OpenCLKernel
	{
	public:

		OpenCLKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName);

		~OpenCLKernel();

		cl_kernel getKernel() const;

	protected:

		void setFloat(UInt32 pIndex, Float pFloat);

		void setFloat2(UInt32 pIndex, const Vector2 &pVector2);

		void setFloat3(UInt32 pIndex, const Vector3 &pVector3);

		void setInt(UInt32 pIndex, Int32 pInteger);

		void setUInt(UInt32 pIndex, UInt32 pInteger);

		void setInt3(UInt32 pIndex, const IntegerVector3<Int32> &pIntegerVector3);

		void setULong(UInt32 pIndex, UInt64 pInteger);

		void setLong2(UInt32 pIndex, const IntegerVector2<Int64> &pIntegerVector2);

		void setLong3(UInt32 pIndex, const IntegerVector3<Int64> &pIntegerVector3);

		void setBuffer(UInt32 pIndex, cl_mem pBuffer);

	private:

		void createProgramWithSourceString(OpenCLManager *pOpenCLManager, const char *pSourceTextString);

		cl_program openCLProgram;
		cl_kernel kernel;
	};

	inline
	OpenCLKernel::OpenCLKernel(OpenCLManager *pOpenCLManager, const TextFileEditor *pTextFileEditor, const char *pKernelName)
	{
		this->createProgramWithSourceString(pOpenCLManager, pTextFileEditor->getTextString());

		// Used to store the return code from OpenCL functions
		cl_int returnCode;

		// Create the kernel
		this->kernel = clCreateKernel(this->openCLProgram, pKernelName, &returnCode);
		FORGE_OPENCL_ERROR_CODE_CHECK(returnCode);
	}

	inline
	OpenCLKernel::~OpenCLKernel()
	{
		FORGE_OPENCL_ERROR_CODE_CHECK(clReleaseKernel(this->kernel));
		FORGE_OPENCL_ERROR_CODE_CHECK(clReleaseProgram(this->openCLProgram));
	}

	inline
	cl_kernel OpenCLKernel::getKernel() const
	{
		return this->kernel;
	}

	inline
	void OpenCLKernel::setFloat(UInt32 pIndex, Float pFloat)
	{
		cl_float floatValue = pFloat;
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_float), &floatValue));
	}

	inline
	void OpenCLKernel::setFloat2(UInt32 pIndex, const Vector2 &pVector2)
	{
		cl_float2 float2Value;
		float2Value.x = static_cast<cl_float>(pVector2.x);
		float2Value.y = static_cast<cl_float>(pVector2.y);
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_float2), &float2Value));
	}

	inline
	void OpenCLKernel::setFloat3(UInt32 pIndex, const Vector3 &pVector3)
	{
		cl_float3 float3Value = convertVector3Tocl_float3(pVector3);
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_float3), &float3Value));
	}

	inline
	void OpenCLKernel::setInt(UInt32 pIndex, Int32 pInt32)
	{
		cl_int intValue = pInt32;
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_int), &intValue));
	}

	inline
	void OpenCLKernel::setUInt(UInt32 pIndex, UInt32 pUInt32)
	{
		cl_uint uintValue = pUInt32;
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_int), &uintValue));
	}

	inline
	void OpenCLKernel::setInt3(UInt32 pIndex, const IntegerVector3<Int32> &pIntegerVector3)
	{
		cl_int3 int3Value;
		int3Value.x = static_cast<cl_int>(pIntegerVector3.x);
		int3Value.y = static_cast<cl_int>(pIntegerVector3.y);
		int3Value.z = static_cast<cl_int>(pIntegerVector3.z);
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_int3), &int3Value));
	}

	inline
	void OpenCLKernel::setULong(UInt32 pIndex, UInt64 pInteger)
	{
		cl_ulong ulongValue = pInteger;
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_ulong), &ulongValue));
	}

	inline
	void OpenCLKernel::setLong2(UInt32 pIndex, const IntegerVector2<Int64> &pIntegerVector2)
	{
		cl_long2 long2Value;
		long2Value.x = static_cast<cl_long>(pIntegerVector2.x);
		long2Value.y = static_cast<cl_long>(pIntegerVector2.y);
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_long2), &long2Value));
	}

	inline
	void OpenCLKernel::setLong3(UInt32 pIndex, const IntegerVector3<Int64> &pIntegerVector3)
	{
		cl_long3 long3Value;
		long3Value.x = static_cast<cl_long>(pIntegerVector3.x);
		long3Value.y = static_cast<cl_long>(pIntegerVector3.y);
		long3Value.z = static_cast<cl_long>(pIntegerVector3.z);
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(cl_long3), &long3Value));
	}

	inline
	void OpenCLKernel::setBuffer(UInt32 pIndex, cl_mem pBuffer)
	{
		FORGE_OPENCL_ERROR_CODE_CHECK(clSetKernelArg(this->kernel, pIndex, sizeof(pBuffer), &pBuffer));
	}
}

#endif