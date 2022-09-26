#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <map>
#include <vector>
#include <functional>



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

	bool initialize();
	int RegisterEventListener(MMEvents MMEvent, std::function<void(MMEvents, void*)> callback );
	bool RemoveEventListener(MMEvents MMEvent, int listenerID );
	bool EventNotification (MMEvents MMEvent, void* sender);


protected:
	std::map<MMEvents, std::map<int, std::function<void(MMEvents, void*)>>> notifications;
	int listenerID;
};
#endif