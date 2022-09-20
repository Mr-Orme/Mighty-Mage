
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "GameObject.h"
#include "Definitions.h"

class TiXmlElement;
class ResourceManager;

class ObjectFactory
{
public:
	std::unique_ptr<GameObject> Create(ObjectFactoryPresets& presets);
	protected:
	
};

#endif