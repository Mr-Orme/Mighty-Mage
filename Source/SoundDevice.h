#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H


#include "SDL_Mixer.h"
#include <string>

class SoundAssetLibrary;
class SoundDevice{
public:
	~SoundDevice();
	SoundDevice(SoundAssetLibrary* sLibrary);
	
	bool PlaySound(std::string sound, int numLoops);
	bool PlaySound(std::string sound, int numLoops, int channel);
	bool stopSounds();
	
	void SetBackground(std::string background);
	void SetSoundLibrary(SoundAssetLibrary* sLibrary){this -> sLibrary = sLibrary;}
	



private:
	SoundAssetLibrary* sLibrary{ nullptr };
	const int numChannels{ 100 };
	
};


#endif