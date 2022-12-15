
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

	std::unique_ptr<GameObject> Create(ObjectFactoryPresets& presets, ResourceManager* devices);
	protected:
	
};

#endif