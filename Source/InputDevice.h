#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "Definitions.h"
#include "SDL.h"
#include <map>
#include <memory>
#include <vector>
class GameObject;
class InputDevice{
public:
	bool initialize();
	void update();
	void Print(SDL_Event* event);

	bool GetEvent(Event event);

private:
	Event Translate(SDL_Event* event);
	SDL_Event* event;
	std::map<Event, bool> keyStates;
};


#endif