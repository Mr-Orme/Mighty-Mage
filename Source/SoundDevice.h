#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H


#include "SDL_Mixer.h"
#include "SoundAssetLibrary.h"
#include <string>

class SoundAssetLibrary;
class SoundDevice{
public:
	~SoundDevice();
	SoundDevice(SoundAssetLibrary* sLibrary);
	
	bool PlaySound(SoundEffect::Event sound, int numLoops, int channel=-1);
	bool stopSounds();
	
	void SetBackground(Locations location);
	void SetSoundLibrary(SoundAssetLibrary* sLibrary){this -> sLibrary = sLibrary;}
	



private:
	SoundAssetLibrary* sLibrary{ nullptr };
	const int numChannels{ 100 };
	
};


#endif