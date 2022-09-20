#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include "GameObject.h"
#include "Definitions.h"

class ResourceManager;
class Component
{
public:
	Component(GameObject* owner, ResourceManager* devices);
	~Component();
	
	virtual bool initialize(ObjectFactoryPresets& presets)=0;
	virtual std::unique_ptr<GameObject> update()=0;
	
	ResourceManager* GetDevices() { return devices; }
	GameObject* getOwner();

protected:
	GameObject* _owner{ nullptr };
	ResourceManager* devices{ nullptr };
};


#endif