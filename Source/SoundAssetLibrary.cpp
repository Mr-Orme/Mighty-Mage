#include "SoundAssetLibrary.h"

//**************************************
//Nothing here, move along.
SoundAssetLibrary::SoundAssetLibrary()
//**************************************
{

}
SoundAssetLibrary::~SoundAssetLibrary()
{
	for(auto [event, effect]: soundEffectLibrary)
	{
		Mix_FreeChunk(effect.sound);
	}
	for (auto [location, music] : musicLibrary)
	{
		Mix_FreeMusic(music);
	}
}

//**************************************
//finds a sound effect in the library, based on the passed name
SoundEffect& SoundAssetLibrary::searchSoundEffects(SoundEffect::Event event)
//**************************************
{
	return soundEffectLibrary.find(event) -> second;
}
//**************************************
//adds a sound effect to the sound effect library
bool SoundAssetLibrary::addSoundEffect(SoundEffect::Event event, std::string path, int timeBetweenPlays)
//**************************************
{
	SoundEffect theSound { Mix_LoadWAV(path.c_str()), (unsigned int)timeBetweenPlays, 0 };
	soundEffectLibrary.emplace(std::make_pair(event, theSound));
		return true;
	
}

//**************************************
//finds the background music based on a string and returns it.
Mix_Music* SoundAssetLibrary::searchMusic(Locations location)
//**************************************
{
	
	//make sure we found one.
	if(auto musicIter{ musicLibrary.find(location) }; musicIter == musicLibrary.end())
	{
		{printf("Background File not found!") ;}
		return nullptr;
	}
	else return musicIter -> second;
}

//**************************************
//adds background music to the library
bool SoundAssetLibrary::addBackgroundMusic(Locations location, std::string path)
//**************************************
{
	//Mix_Load
	if(musicLibrary[location] = Mix_LoadMUS(path.c_str())) return true;
	return false;
}
