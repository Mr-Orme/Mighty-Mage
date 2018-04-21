#ifndef TIMER_H
#define TIMER_H

#include"SDL.h"
#include"Definitions.h"

class Timer
{
public:
	//Initializes variables
	Timer();
	bool Initialize(GAME_INT);

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	GAME_INT getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();
	
	//Regulate
	void fpsRegulate();

private:
	//The clock time when the timer started
	GAME_INT startTicks;
	GAME_FLT mpf; //millisecond per frame

	//The ticks stored when the timer was paused
	GAME_INT pausedTicks;

	//The timer status
	bool paused;
	bool started;
};

#endif