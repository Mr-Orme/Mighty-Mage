#ifndef SOUNDASSETLIBRARY_H
#define SOUNDASSETLIBRARY_H

#include <map>
#include <string>
#include "Definitions.h"

#include "SoundDevice.h"

class SoundAssetLibrary{
public:
	SoundAssetLibrary();
	~SoundAssetLibrary();
	//TODO::see if I can get rid of this initialize method.
	bool initialize(SoundDevice* sDevice);
	//TODO::reduce the reliance on strings in these libraries..
	Mix_Chunk* searchSoundEffects(std::string name);
	Mix_Music* searchMusic(std::string name);
	bool addSoundEffect(std::string name, std::string path);
	bool addBackgroundMusic(std::string name, std::string path);
	bool removeAsset(std::string name);
private:
	std::map<std::string, Mix_Chunk* > soundEffectLibrary;
	std::map<std::string, Mix_Music* > musicLibrary;
	SoundDevice* sDevice{nullptr};

};




#endif