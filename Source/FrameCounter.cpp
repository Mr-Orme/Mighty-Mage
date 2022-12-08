#include "FrameCounter.h"
#include <limits>
namespace FrameCounter
{
	namespace {
		unsigned int frameNumber{ 0 };
	}
	unsigned int framesSince(unsigned int pastFrameCount)
	{
		//deal with wraparound.
		if (pastFrameCount < frameNumber)
		{
			return frameNumber - pastFrameCount;
		}
		else
		{
			return frameNumber + (UINT_MAX - pastFrameCount);
		}
		return 0;
	}

	unsigned int currentFrame()
	{
		return frameNumber;
	}

	void incrementFrame()
	{
		frameNumber++;
	}

}