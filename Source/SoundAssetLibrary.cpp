#include "SoundAssetLibrary.h"

//**************************************
//Nothing here, move along.
SoundAssetLibrary::SoundAssetLibrary()
//**************************************
{

}
SoundAssetLibrary::~SoundAssetLibrary()
{
	//iterates through sound effects.
	std::map<std::string, SoundEffect>::iterator soundIter;
	for (soundIter = soundEffectLibrary.begin(); soundIter != soundEffectLibrary.end(); soundIter++)
	{
		//removes sound effect.
		Mix_FreeChunk(soundIter->second.sound);
	}
	//itersates through music.
	std::map<std::string, Mix_Music* >::iterator musicIter;
	for (musicIter = musicLibrary.begin(); musicIter != musicLibrary.end(); musicIter++)
	{
		//removes music.
		Mix_FreeMusic(musicIter->second);
	}
}

//**************************************
//finds a sound effect in the library, based on the passed name
SoundEffect& SoundAssetLibrary::searchSoundEffects(std::string name)
//**************************************
{
	return soundEffectLibrary.find(name) -> second;
}
//**************************************
//adds a sound effect to the sound effect library
bool SoundAssetLibrary::addSoundEffect(std::string name, std::string path, int timeBetweenPlays)
//**************************************
{
	SoundEffect theSound { Mix_LoadWAV(path.c_str()), timeBetweenPlays, 0.0f };
	soundEffectLibrary.emplace(std::make_pair(name, theSound));
		return true;
	
}
//**************************************
//Have not set this up yet, as there has bee no need for it.
bool SoundAssetLibrary::removeAsset(std::string name)
//**************************************
{
	return true;
}
//**************************************
//finds the background music based on a string and returns it.
Mix_Music* SoundAssetLibrary::searchMusic(std::string name)
//**************************************
{
	std::map<std::string, Mix_Music* >::iterator musicIter;
	musicIter = musicLibrary.find(name);
	//make sure we found one.
	if(musicIter == musicLibrary.end())
	{
		{printf("Background File not found!") ;}
		return nullptr;
	}
	else return musicIter -> second;
}

//**************************************
//adds background music to the library
bool SoundAssetLibrary::addBackgroundMusic(std::string name, std::string path)
//**************************************
{
	//Mix_Load
	if(musicLibrary[name] = Mix_LoadMUS(path.c_str())) return true;
	return false;
}
