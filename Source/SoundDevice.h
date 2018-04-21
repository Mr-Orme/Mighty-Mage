#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include "Definitions.h"
#include "SDL_Mixer.h"
#include <string>

class SoundAssetLibrary;
class SoundDevice{
public:
	~SoundDevice();
	bool Initialize();
	void Shutdown();

	bool PlaySound(std::string sound, int numLoops);
	bool PlaySound(std::string sound, int numLoops, int channel);
	
	void SetBackground(std::string background);
	void SetSoundLibrary(SoundAssetLibrary* sLibrary){this -> sLibrary = sLibrary;}
	



private:
	SoundAssetLibrary* sLibrary;
};


#endif