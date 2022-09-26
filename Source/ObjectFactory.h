
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H
#include <string>
#include "Vector2D.h"
#include "Definitions.h"
#include "Initializers.h"

class GameObject;
class ResourceManager;




class ObjectFactory
{
public:
	//struct Presets
	//{
	//	std::string objectType;
	//	Vector2D position;
	//	float angle;
	//	int health;//move to new AssetLibraryGame
	//	int destructTime;//move to new AssetLibraryGame
	//	ResourceManager* devices;
	//	std::map<Direction, bool> gDirection;
	//	//std::unique_ptr<GameObject> joinedObject; //only needed for joint
	//};

	std::unique_ptr<GameObject> Create(ObjectFactoryPresets& presets);
	protected:
	
};

#endif