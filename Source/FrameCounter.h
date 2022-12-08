#pragma once

namespace FrameCounter
{
	unsigned int framesSince(unsigned int pastFrameCount);
	unsigned int currentFrame();
	void incrementFrame();

};
