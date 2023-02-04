#ifndef FORGE_MUTEX_H
#define FORGE_MUTEX_H

#include "../ForgeWindowsHeader.h"
#include <Forge/Source/Platform/Windows/WindowsLogging.h>

namespace forge
{
	class Mutex
	{
	public:

		Mutex();

		~Mutex();

		void lock();

		void unlock();

	private:

		CRITICAL_SECTION criticalSection;
	};

	inline
	Mutex::Mutex()
	{
		if(!InitializeCriticalSectionAndSpinCount(&this->criticalSection, 1024))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("InitializeCriticalSectionAndSpinCount function failed");
		}
	}

	inline
	Mutex::~Mutex()
	{
		DeleteCriticalSection(&this->criticalSection);
	}

	inline
	void Mutex::lock()
	{
		EnterCriticalSection(&this->criticalSection);
	}

	inline
	void Mutex::unlock()
	{
		LeaveCriticalSection(&this->criticalSection);
	}
}

#endif