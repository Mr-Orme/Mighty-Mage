#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H


#include "SDL_Mixer.h"
#include "Definitions.h"
#include "SoundAssetLibrary.h"
#include <string>

enum class Levels;
class SoundDevice{
public:
	~SoundDevice() {}
	SoundDevice(SoundAssetLibrary* sLibrary);
	
	bool PlaySound(SoundEffect::Event sound, int numLoops, int channel=-1);
	bool stopSounds();
	
	void SetBackground(Levels location);
	void SetSoundLibrary(SoundAssetLibrary* sLibrary);
	



private:
	SoundAssetLibrary* sLibrary{ nullptr };
	const int numChannels{ 100 };
	
};


#endif