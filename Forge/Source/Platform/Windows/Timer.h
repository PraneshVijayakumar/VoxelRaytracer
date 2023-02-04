#ifndef FORGE_TIMER_H
#define FORGE_TIMER_H

#include "ForgeWindowsHeader.h"
#include <Forge/Source/Core/Forge.h>

namespace forge
{
	class Timer
	{
	public:

		Timer();

		// This will cause the timer to count the microseconds from 0 again
		void reset();

		// Returns the number of microseconds elapsed since this timer object was created,
		// or the last call to the reset function of this timer object, whichever happend more recently
		Int64 getTimePassed() const;

	private:

		static LARGE_INTEGER frequency;
		LARGE_INTEGER startingTime;
	};

	inline
	Timer::Timer()
	{
		this->reset();
	}
}

#endif