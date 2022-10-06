#include <iostream>
#include "InputDevice.h"

InputDevice::InputDevice():
	event(std::make_unique<SDL_Event>())
{
	if (!event) {
		printf("SDL Inputs could not initialize!");
		exit(1);
	}

	keyStates[Inputs::down] = false;
	keyStates[Inputs::up] = false;
	keyStates[Inputs::left] = false;
	keyStates[Inputs::right] = false;
	keyStates[Inputs::space] = false;
	keyStates[Inputs::quit] = false;
	keyStates[Inputs::shift] = false;
	keyStates[Inputs::key_b] = false;
	keyStates[Inputs::key_y] = false;
	keyStates[Inputs::key_n] = false;
	update();
}

//**************************************
//Updates the event based on the passed in SDL_EVENT
void InputDevice::update()
//**************************************
{
	Inputs gEvent;
	
	if(SDL_PollEvent(event.get()))
	{
		gEvent = Translate(event.get());
		if (gEvent != Inputs::NA)
		{
			//updates the proper key state based on the event that was passed in
			switch (event->type)
			{
			case SDL_KEYDOWN:
				//translates the SDL even to a game event.
				keyStates.find(gEvent)->second = true;
				break;
			case SDL_KEYUP:
				//translates the SDL even to a game event.
				keyStates.find(gEvent)->second = false;
				break;
			case SDL_QUIT:
				keyStates.find(gEvent)->second = true;
				break;
			default:
				break;
			}
		}
	}
	
	
	
}

//**************************************
//converts the SDL event to a game event
InputDevice::Inputs InputDevice::Translate(SDL_Event* event)
//**************************************
{
	//This switch is here in case we want to add other events, such as mouse events.
	switch(event->key.keysym.sym)
		{
		case SDLK_LEFT:
			return Inputs::left;
			break;
		case SDLK_RIGHT:
			return Inputs::right;
			break;
		case SDLK_UP:
			return Inputs::up;
			break;
		case SDLK_DOWN:
			return Inputs::down;
			break;	
		case SDLK_SPACE:
			return Inputs::space;
			break;
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			return Inputs::shift;
			break;
		case SDLK_b:
			return Inputs::key_b;
			break;
		case SDLK_y:
			return Inputs::key_y;
			break;
		case SDLK_n:
			return Inputs::key_n;
			break;
		}
	return Inputs::NA;
}

//**************************************
//returns the state of a key
bool InputDevice::isPressed(Inputs key)
//**************************************
{
	return (keyStates.find(key) -> second);
}

//**************************************
//Prints out the current event
void InputDevice::Print(SDL_Event* event)
//**************************************
{
	
	switch(event -> type){
	case SDL_KEYDOWN:
		std::cout << "SDL event Type: KEYDOWN" << std::endl <<
			"Key Pressed: " << event -> key.keysym.sym << std::endl;
	}
	
}