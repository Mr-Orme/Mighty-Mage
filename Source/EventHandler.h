#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <map>
#include <vector>
#include <functional>

#include "Definitions.h"

class EventHandler{
public:
	enum MMEvents 
	{
		MM_HIT_WALL, MM_LEVEL_CHANGE, 
		MM_NOTICE, MM_PICKUP_ITEM, 
		MM_ENCOUNTER, MM_STAT_CHANGE, 
		MM_CHOICE, MM_QUEST, 
		MM_NUM_EVENTS 
	};

	bool Initialize();
	GAME_INT RegisterEventListener(MMEvents MMEvent, std::function<void(MMEvents, void*)> callback );
	bool RemoveEventListener(MMEvents MMEvent, GAME_INT listenerID );
	bool EventNotification (MMEvents MMEvent, void* sender);


protected:
	std::map<MMEvents, std::map<GAME_INT, std::function<void(MMEvents, void*)>>> notifications;
	GAME_INT listenerID;
};
#endif