
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H


#include "Definitions.h"

class TiXmlElement;
class ResourceManager;
class GameObject;

class ObjectFactory
{
public:
	struct GAME_OBJECTFACTORY_PRESETS
	{
		std::string objectType;
		GAME_VEC position;
		GAME_FLT angle;
		GAME_INT walkSpeed;
		GAME_INT runMultiplier;
		GAME_INT health;//move to new AssetLibraryGame
		GAME_INT destructTime;//move to new AssetLibraryGame
		ResourceManager* devices;
		std::map<GAME_DIRECTION, bool> gDirection;
		//std::shared_ptr<GameObject> joinedObject; //only needed for joint
	};

	struct GAME_OBJECT_STATS
	{
		int health;
	};
	GameObject* Create(GAME_OBJECTFACTORY_PRESETS& presets);
	protected:
	
};

#endif