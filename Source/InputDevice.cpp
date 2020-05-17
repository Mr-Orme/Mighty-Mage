#include <iostream>
#include "InputDevice.h"

//**************************************
//Sets all keystates to false. sets initial event to NA
//NA should be passed in, but can be any valid event.
bool InputDevice::Initialize()
//**************************************
{
	//========================================
	//Construct Event System
	//========================================
	event = new SDL_Event();
	if(!event){
		printf( "SDL Event could not initialize!");
		exit(1);
	}

	keyStates[GAME_DOWN] = false;
	keyStates[GAME_UP] = false;
	keyStates[GAME_LEFT] = false;
	keyStates[GAME_RIGHT] = false;
	keyStates[GAME_SPACE] = false;
	keyStates[GAME_QUIT] = false;
	keyStates[GAME_SHIFT] = false;
	keyStates[GAME_B] = false;
	Update();
	return true;
}

//**************************************
//Updates the event based on the passed in SDL_EVENT
void InputDevice::Update()
//**************************************
{
	GAME_EVENT gEvent;
	if(SDL_PollEvent(event))
	{
		//updates the proper key state based on the event that was passed in
		switch (event -> type)
		{
		case SDL_KEYDOWN:
			//translates the SDL even to a game event.
			gEvent = Translate(event);
			keyStates.find(gEvent) -> second = true;
			break;
		case SDL_KEYUP:
			//translates the SDL even to a game event.
			gEvent = Translate(event);
			keyStates.find(gEvent) -> second = false;
			break;
		case SDL_QUIT:
			keyStates.find(GAME_QUIT) -> second = true;
			break;
		default:
			break;
		}
	}
	
	
	
}

//**************************************
//converts the SDL event to a game event
GAME_EVENT InputDevice::Translate(SDL_Event* event)
//**************************************
{
	//This switch is here in case we want to add other events, such as mouse events.
	switch(event->key.keysym.sym)
		{
		case SDLK_LEFT:
			return GAME_LEFT;
			break;
		case SDLK_RIGHT:
			return GAME_RIGHT;
			break;
		case SDLK_UP:
			return GAME_UP;
			break;
		case SDLK_DOWN:
			return GAME_DOWN;
			break;	
		case SDLK_SPACE:
			return GAME_SPACE;
			break;
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			return GAME_SHIFT;
			break;
		case SDLK_b:
			return GAME_B;
			break;
		}



	return GAME_NA;
}

//**************************************
//returns the state of a key
bool InputDevice::GetEvent(GAME_EVENT event)
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