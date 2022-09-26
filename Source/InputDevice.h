#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H


#include "SDL.h"
#include <map>
#include <memory>
#include <vector>
class GameObject;
class InputDevice{
public:
	enum class Inputs { NA, up, down, left, right, space, shift, key_b, quit, numEvents };
	InputDevice();
	
	void update();
	void Print(SDL_Event* event);

	bool isPressed(Inputs key);

private:
	Inputs Translate(SDL_Event* event);
	std::unique_ptr<SDL_Event> event;
	std::map<Inputs, bool> keyStates;
};


#endif