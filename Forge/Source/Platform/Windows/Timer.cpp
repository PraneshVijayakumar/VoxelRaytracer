#include "Timer.h"
#include <Forge/Source/Platform/Windows/WindowsLogging.h>

namespace forge
{
	LARGE_INTEGER Timer::frequency;

	void Timer::reset()
	{
		static bool frequencyQueried = false;
		if(!frequencyQueried)
		{
			// On Windows XP or later, the QueryPerformanceFrequency function will always succeed and never return 0.
			// Checking the return value here just in case
			if(!QueryPerformanceFrequency(&frequency))
			{
				FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("QueryPerformanceFrequency function failed");
			}
		}

		// On Windows XP or later, the QueryPerformanceCounter function will always succeed and never return 0.
		// Checking the return value here just in case
		if(!QueryPerformanceCounter(&this->startingTime))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("QueryPerformanceCounter function failed");
		}
	}

	Int64 Timer::getTimePassed() const
	{
		LARGE_INTEGER currentTime;

		// On Windows XP or later, the QueryPerformanceCounter function will always succeed and never return 0.
		// Checking the return value here just in case
		if(!QueryPerformanceCounter(&currentTime))
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("QueryPerformanceCounter function failed");
		}

		// Convert the elapsed number of ticks to microseconds before dividing by the frequency, to guard against loss of precision
		return (currentTime.QuadPart - this->startingTime.QuadPart) * 1000000 / this->frequency.QuadPart;
	}
}