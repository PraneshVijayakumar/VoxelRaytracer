#ifndef FORGE_FPS_COUNTER_H
#define FORGE_FPS_COUNTER_H

#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/Containers/Queue.h>

namespace forge
{
	class FPSCounter
	{
	public:

		FPSCounter();

		SizeType getCurrentFPS() const;

		void simulate(Int64 pNewFrameCompleteTime);

	private:

		Queue<Int64> frameCompleteTimes;
		SizeType currentFPS;
	};

	inline
	FPSCounter::FPSCounter() :
	frameCompleteTimes(1000),
	currentFPS(0)
	{
	}

	inline
	SizeType FPSCounter::getCurrentFPS() const
	{
		return this->currentFPS;
	}

	inline
	void FPSCounter::simulate(Int64 pNewFrameCompleteTime)
	{
		if(this->frameCompleteTimes.getNumberOfElements() == this->frameCompleteTimes.getMaxNumberOfElements())
		{
			this->frameCompleteTimes.remove();
		}

		this->frameCompleteTimes.append(pNewFrameCompleteTime);

		// Make sure that the frame complete times queue holds only data from the last second
		{
			while(this->frameCompleteTimes.getNumberOfElements() > 1 &&
				  this->frameCompleteTimes.getLastElement() - this->frameCompleteTimes.getFirstElement() > 1000000)
			{
				this->frameCompleteTimes.remove();
			}
		}

		if(this->frameCompleteTimes.getNumberOfElements() > 1)
		{
			// The total number of frames rendered in the last second, approximately
			const SizeType numberOfFrames = this->frameCompleteTimes.getNumberOfElements() - 1;
			
			// The total time it took to render all of the frames
			Int64 totalTimeInMicroseconds = 0;
			for(SizeType frameCompleteTimeIndex = 1; frameCompleteTimeIndex < this->frameCompleteTimes.getNumberOfElements(); ++frameCompleteTimeIndex)
			{
				totalTimeInMicroseconds += this->frameCompleteTimes[frameCompleteTimeIndex] - this->frameCompleteTimes[frameCompleteTimeIndex - 1];
			}
			
			const Double totalTimeInSeconds = static_cast<Double>(totalTimeInMicroseconds) / 1000000.0;

			this->currentFPS = roundDoubleToInteger<SizeType>(static_cast<Double>(numberOfFrames) / totalTimeInSeconds);
		}
		else
		{
			this->currentFPS = 0;
		}
	}
}

#endif