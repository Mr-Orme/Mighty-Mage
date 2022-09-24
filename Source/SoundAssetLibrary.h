#ifndef SOUNDASSETLIBRARY_H
#define SOUNDASSETLIBRARY_H

#include <map>
#include <string>
#include "Definitions.h"

#include "SoundDevice.h"
//TODO::SoundEffects have a time between plays!
struct SoundEffect
{
	Mix_Chunk* sound{ nullptr };
	unsigned int timeBetweenPlays{ 0 };
	unsigned int lastPlayed{ 0 };
};
class SoundAssetLibrary{
public:
	SoundAssetLibrary();
	~SoundAssetLibrary();
	//TODO::see if I can get rid of this initialize method.
	bool initialize(SoundDevice* sDevice);
	//TODO::reduce the reliance on strings in these libraries..
	SoundEffect& searchSoundEffects(std::string name);
	Mix_Music* searchMusic(std::string name);
	bool addSoundEffect(std::string name, std::string path, int timeBetweenPlays=0);
	bool addBackgroundMusic(std::string name, std::string path);
	bool removeAsset(std::string name);
private:
	std::map<std::string, SoundEffect > soundEffectLibrary;
	std::map<std::string, Mix_Music* > musicLibrary;
	SoundDevice* sDevice{nullptr};

};




#endif