#ifndef SOUNDASSETLIBRARY_H
#define SOUNDASSETLIBRARY_H

#include <map>
#include <string>
#include "SDL_Mixer.h"
#include "Definitions.h"
enum class Levels;
struct SoundEffect
{
	enum class Event { walk, run, hitWall, pickup, background };
	Mix_Chunk* sound{ nullptr };
	unsigned int timeBetweenPlays{ 0 };
	unsigned int lastPlayed{ 0 };
};


class SoundAssetLibrary{
public:
	SoundAssetLibrary();
	~SoundAssetLibrary();
			
	SoundEffect& searchSoundEffects(SoundEffect::Event event);
	Mix_Music* searchMusic(Levels location);
	bool addSoundEffect(SoundEffect::Event event, std::string path, int timeBetweenPlays=0);
	bool addBackgroundMusic(Levels location, std::string path);
	
private:
	std::map<SoundEffect::Event, SoundEffect > soundEffectLibrary;
	std::map<Levels, Mix_Music* > musicLibrary;
};




#endif