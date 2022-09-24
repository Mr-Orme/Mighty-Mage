#include "FrameCounter.h"
#include <limits>
unsigned int FrameCounter::frameNumber{ 0 };
unsigned FrameCounter::framesSince(unsigned int pastFrameCount)
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

unsigned int FrameCounter::currentFrame()
{
	return frameNumber;
}

void FrameCounter::incrementFrame()
{
	frameNumber++;
}
