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

	bool GetEvent(GAME_EVENT event);

private:
	GAME_EVENT Translate(SDL_Event* event);
	SDL_Event* event;
	std::map<GAME_EVENT, bool> keyStates;
};


#endif