#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H


#include "SDL.h"
#include <map>
#include <memory>
#include <vector>
class GameObject;
class InputDevice{
public:
	InputDevice();
	
	void update();
	void Print(SDL_Event* event);

	bool GetEvent(Event event);

private:
	Event Translate(SDL_Event* event);
	std::unique_ptr<SDL_Event> event;
	std::map<Event, bool> keyStates;
};


#endif