#include "SoundAssetLibrary.h"

//set's the sound device, and set's the sound library in the sound device.
bool SoundAssetLibrary::Initialize(SoundDevice* sDevice)
//**************************************
{
	this -> sDevice = sDevice;
	sDevice -> SetSoundLibrary(this);
	return true;
}
//**************************************
//finds a sound effect in the library, based on the passed name
Mix_Chunk* SoundAssetLibrary::SearchSoundEffects(std::string name)
//**************************************
{
	return soundEffectLibrary.find(name) -> second;
}
//**************************************
//adds a sound effect to the sound effect library
bool SoundAssetLibrary::AddSoundEffect(std::string name, std::string path)
//**************************************
{
	if(soundEffectLibrary[name] = Mix_LoadWAV(path.c_str())) return true;
	return false;
	
}
//**************************************
//Have not set this up yet, as there has bee no need for it.
bool SoundAssetLibrary::RemoveAsset(std::string name)
//**************************************
{
	return true;
}
//**************************************
//finds the background music based on a string and returns it.
Mix_Music* SoundAssetLibrary::SearchMusic(std::string name)
//**************************************
{
	std::map<std::string, Mix_Music* >::iterator musicIter = musicLibrary.find(name);
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
bool SoundAssetLibrary::AddBackgroundMusic(std::string name, std::string path)
//**************************************
{
	//Mix_Load
	if(musicLibrary[name] = Mix_LoadMUS(path.c_str())) return true;
	return false;
}

//**************************************
//cleans out the libraries by unloading all sounds from the mixer.
bool SoundAssetLibrary::Finish()
//**************************************
{
	//iterates through sound effects.
	
	for (auto sound : soundEffectLibrary )
	{
		//removes sound effect.
		Mix_FreeChunk(sound.second);
	}
	//itersates through music.
	for (auto song : musicLibrary)
	{
		//removes music.
		Mix_FreeMusic(song.second);
	}
	return true;
}