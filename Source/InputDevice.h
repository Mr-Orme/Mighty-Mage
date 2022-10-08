#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H


#include "SDL.h"
#include <map>
#include <memory>
#include <vector>
class GameObject;
enum class Inputs { NA, up, down, left, right, space, shift, key_b, key_y, key_n, quit, numEvents };
class InputDevice{
public:
	
	InputDevice();
	
	void update();
	bool isPressed(Inputs key);

	void Print(SDL_Event* event);

private:
	Inputs Translate(SDL_Event* event);
	std::unique_ptr<SDL_Event> event;
	std::map<Inputs, bool> keyStates;
};


#endif