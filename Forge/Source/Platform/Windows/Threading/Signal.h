#ifndef FORGE_SIGNAL_H
#define FORGE_SIGNAL_H

#include "../ForgeWindowsHeader.h"
#include <Forge/Source/Platform/Windows/WindowsLogging.h>

namespace forge
{
	class Signal
	{
	public:

		Signal(const bool &pInitialState = true, const bool &pManualReset = false);

		bool wait(UInt32 pWaitTime = INFINITE);

		void send();

	private:

		HANDLE eventHandle;
	};

	inline
	Signal::Signal(const bool &pInitialState, const bool &pManualReset)
	{
		this->eventHandle = CreateEventW(NULL, (pManualReset == TRUE)? true : false, (pInitialState == TRUE)? true : false, NULL);
		if(this->eventHandle == NULL)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("CreateEventW function failed");
		}
	}

	inline
	bool Signal::wait(UInt32 pWaitTime)
	{
		DWORD returnValue = WaitForSingleObject(this->eventHandle, pWaitTime);

		if(returnValue == WAIT_OBJECT_0)
		{
			return true;
		}
		else if(returnValue == WAIT_FAILED)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("WaitForSingleObject function failed when called on a Windows Event Object");
			return false;
		}
		else
		{
			return false;
		}
	}

	inline
	void Signal::send()
	{
		SetEvent(this->eventHandle);
	}
}

#endif