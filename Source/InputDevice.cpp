#include <iostream>
#include "InputDevice.h"

InputDevice::InputDevice():
	event(std::make_unique<SDL_Event>())
{
	if (!event) {
		printf("SDL Event could not initialize!");
		exit(1);
	}

	keyStates[Event::down] = false;
	keyStates[Event::up] = false;
	keyStates[Event::left] = false;
	keyStates[Event::right] = false;
	keyStates[Event::space] = false;
	keyStates[Event::quit] = false;
	keyStates[Event::shift] = false;
	keyStates[Event::key_b] = false;
	update();
}

//**************************************
//Updates the event based on the passed in SDL_EVENT
void InputDevice::update()
//**************************************
{
	Event gEvent;
	
	if(SDL_PollEvent(event.get()))
	{
		gEvent = Translate(event.get());
		if (gEvent != Event::NA)
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
Event InputDevice::Translate(SDL_Event* event)
//**************************************
{
	//This switch is here in case we want to add other events, such as mouse events.
	switch(event->key.keysym.sym)
		{
		case SDLK_LEFT:
			return Event::left;
			break;
		case SDLK_RIGHT:
			return Event::right;
			break;
		case SDLK_UP:
			return Event::up;
			break;
		case SDLK_DOWN:
			return Event::down;
			break;	
		case SDLK_SPACE:
			return Event::space;
			break;
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			return Event::shift;
			break;
		case SDLK_b:
			return Event::key_b;
			break;
		}
	return Event::NA;
}

//**************************************
//returns the state of a key
bool InputDevice::GetEvent(Event event)
//**************************************
{
	return (keyStates.find(event) -> second);
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