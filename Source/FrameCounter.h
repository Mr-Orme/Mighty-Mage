#pragma once

class FrameCounter
{
public:
	static unsigned int framesSince(unsigned int pastFrameCount);
	static unsigned int currentFrame();
	static void incrementFrame();
private:
	static unsigned int frameNumber;

};
