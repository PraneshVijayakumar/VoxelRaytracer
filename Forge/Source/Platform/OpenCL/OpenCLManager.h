#ifndef FORGE_OPENCL_MANAGER_H
#define FORGE_OPENCL_MANAGER_H

#include <CL/cl.h>

namespace forge
{
	class OpenCLManager
	{
	public:

		void startup();

		void shutdown();

		cl_device_id getDeviceID() const;

		cl_context getContext() const;

		cl_command_queue getCommandQueue() const;

	private:

		cl_platform_id platformID;
		cl_device_id deviceID;
		cl_context context;
		cl_command_queue commandQueue;
	};

	inline
	cl_device_id OpenCLManager::getDeviceID() const
	{
		return this->deviceID;
	}

	inline
	cl_context OpenCLManager::getContext() const
	{
		return this->context;
	}

	inline
	cl_command_queue OpenCLManager::getCommandQueue() const
	{
		return this->commandQueue;
	}
}

#endif