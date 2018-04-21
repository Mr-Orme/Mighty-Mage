#include "EventHandler.h"

bool EventHandler::Initialize()
{
	listenerID = 0;
	return true;
}
GAME_INT EventHandler::RegisterEventListener(MMEvents MMEvent, std::function<void(MMEvents, void*)> callback)
{
	//save the callback function to the appropriate map
	//First dimension is the event type
	//second is the ID of the listener
	notifications[MMEvent][listenerID] = callback;
	//we've used this ID, move on to the next one.
	listenerID++;

	//-1 because we just ++'ed the listenerID
	return listenerID-1;
}
bool EventHandler::RemoveEventListener(MMEvents MMEvent, GAME_INT removeMe)
{
	//erases the listener based on the event & ID passed.
	notifications[MMEvent].erase(notifications[MMEvent].find(removeMe));	
	return true;	
}
bool EventHandler::EventNotification (MMEvents MMEvent, void* sender)
{
	//for each listener assigned to this event
	for(auto listener : notifications[MMEvent])
	{
		//call the assigned function.
		listener.second(MMEvent, sender);
	}
	return true;
}