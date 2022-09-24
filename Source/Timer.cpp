#include <iostream>

#include "timer.h"

Timer::Timer(int FPS) :mpf(1000.0f / FPS)
{}

void Timer::start()
{
    started = true;
    paused = false;
     
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    started = false;
    paused = false;
}

void Timer::pause()
{
    if( started  && !paused )
    {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    if(paused)
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

int Timer::getTicks()
{
    if(started)
    {
        if( paused)
        {
            return pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - startTicks;
        }
    }

     return 0;
}

bool Timer::isStarted()
{
    return started;
}

bool Timer::isPaused()
{
    return paused;
}

void Timer::fpsRegulate()
{

	if(getTicks() < mpf)
	{
		SDL_Delay((int)mpf - getTicks());
	}
}