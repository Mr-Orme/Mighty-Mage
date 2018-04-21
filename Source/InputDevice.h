#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "Definitions.h"
#include "SDL.h"
#include <map>
class InputDevice{
public:
	bool Initialize();
	void Update();
	void Print(SDL_Event* event);
	enum GAME_EVENT
	{
		GAME_NA,
		GAME_UP,
		GAME_DOWN,
		GAME_LEFT,
		GAME_RIGHT,
		GAME_SPACE,
		GAME_SHIFT,
		GAME_B,
		GAME_QUIT,
		GAME_NUM_EVENTS
	};

	bool GetEvent(GAME_EVENT event);

private:
	GAME_EVENT Translate(SDL_Event* event);
	SDL_Event* event;
	std::map<GAME_EVENT, bool> keyStates;
};


#endif