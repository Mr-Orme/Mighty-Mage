#include "SoundDevice.h"
#include "SoundAssetLibrary.h"
#include "Timer.h"
#include "FrameCounter.h"
SoundDevice::~SoundDevice()
{
	Shutdown();
}
SoundDevice::SoundDevice()
{

}
//**************************************
//set's up initial setting for sound device
bool SoundDevice::initialize()
//**************************************
{
	//allows for OGG support
	int flags = MIX_INIT_OGG;
	int initted = Mix_Init(flags);

	if(initted&flags != flags)
	{
		printf("Mix_Init: Failed to init required ogg and mod support!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
	}

	//Load the Mixer subsystem
	if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096 ) <0)
	{
		printf( "SDL Mixer could not initialize! SDL_Error: %s\n", Mix_GetError() );
		exit(1);	    		
	}

	//Mixing parameters
    int numChannels = 100; //Number of concurrent channels to mix
    
    //Allocate sufficient channels
    Mix_AllocateChannels(numChannels);
}

//**************************************
//plays a sound for a number of loops, must be in the sound library.
bool SoundDevice::PlaySound(std::string sound, int numLoops)
//**************************************
{
	int channelID = -1; //Select first available channel
	PlaySound(sound, numLoops, channelID);
	return true;
}
//**************************************
//same as above, but here we can specify a channel to play the sound on.
bool SoundDevice::PlaySound(std::string sound, int numLoops, int channel)
//**************************************
{
	//TODO::update timer to allow for time passed on these..
	auto& [theSound, timeBetweenPlays, lastPlayed] = sLibrary->searchSoundEffects(sound);
	
	if (FrameCounter::framesSince(lastPlayed) > timeBetweenPlays)
	{
		lastPlayed = FrameCounter::currentFrame();
		Mix_PlayChannel(channel, theSound, numLoops);
	}
	return true;
}
//**************************************
//set's the background music to play.
void SoundDevice::SetBackground(std::string background)
//**************************************
{
	Mix_VolumeMusic(.5 * MIX_MAX_VOLUME);
	if(Mix_PlayMusic(sLibrary -> searchMusic(background), -1) == -1)
	{printf("Mix_PlayMusic: %s\n", Mix_GetError());}
}
void SoundDevice::Shutdown()
{
	
}