#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

#include "Definitions.h"

class GameObject;
class ResourceManager;

class Component
{
public:
	Component(GameObject* owner, ResourceManager* devices);
	~Component();
	
	virtual bool initialize(ObjectFactoryPresets& presets)=0;
	virtual std::unique_ptr<GameObject> update()=0;
	//TODO::try and eliminate getDevices
	ResourceManager* getDevices() const { return devices; }
	GameObject* getOwner() const {return(_owner);}

protected:
	GameObject* _owner{ nullptr };
	ResourceManager* devices{ nullptr };
};


#endif