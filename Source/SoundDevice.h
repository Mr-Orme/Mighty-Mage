#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include <string>

#include "SDL_Mixer.h"

#include "Definitions.h"



class ResourceManager;

class SoundDevice{
public:
	~SoundDevice();
	bool Initialize(ResourceManager* devices);
	void Shutdown();

	bool PlaySound(std::string sound, int numLoops);
	bool PlaySound(std::string sound, int numLoops, int channel);
	
	void SetBackground(std::string background);
	
private:
	ResourceManager* devices;
};


#endif