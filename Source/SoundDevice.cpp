#include "SoundDevice.h"
#include "SoundAssetLibrary.h"
#include "Timer.h"
#include "FrameCounter.h"
#include "ResourceManager.h"

SoundDevice::SoundDevice(SoundAssetLibrary* sLibrary):sLibrary(sLibrary)
{
	//allows for OGG support
	int flags = MIX_INIT_OGG;
	int initted = Mix_Init(flags);

	if ((initted & flags) != flags)
	{
		printf("Mix_Init: Failed to init required ogg and mod support!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
	}

	//Load the Mixer subsystem
	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		printf("SDL Mixer could not initialize! SDL_Error: %s\n", Mix_GetError());
		//exit(1);	    		
	}

	//Allocate sufficient channels
	Mix_AllocateChannels(numChannels);
}

//**************************************
//same as above, but here we can specify a channel to play the sound on.
bool SoundDevice::PlaySound(SoundEffect::Event sound, int numLoops, int channel)
//**************************************
{
	auto& [theSound, timeBetweenPlays, lastPlayed] = sLibrary->searchSoundEffects(sound);
	
	if (FrameCounter::framesSince(lastPlayed) > timeBetweenPlays)
	{
		lastPlayed = FrameCounter::currentFrame();
		Mix_PlayChannel(channel, theSound, numLoops);
	}
	return true;
}
bool SoundDevice::stopSounds()
{
	return Mix_HaltChannel(-1);
}
//**************************************
//set's the background music to play.
void SoundDevice::SetBackground(Levels location)
//**************************************
{
	Mix_VolumeMusic((int)(0.5f * MIX_MAX_VOLUME));
	if(Mix_PlayMusic(sLibrary -> searchMusic(location), -1) == -1)
	{printf("Mix_PlayMusic: %s\n", Mix_GetError());}
}

void SoundDevice::SetSoundLibrary(SoundAssetLibrary* sLibrary)
{
	this->sLibrary = sLibrary;
}
