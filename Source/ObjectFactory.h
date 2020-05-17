
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "GameObject.h"
#include "Definitions.h"

class TiXmlElement;
class ResourceManager;

class ObjectFactory
{
public:
	std::shared_ptr<GameObject> Create(GAME_OBJECTFACTORY_PRESETS& presets);
	protected:
	
};

#endif