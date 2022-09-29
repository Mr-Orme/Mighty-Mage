#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <vector>

#include "Vector2D.h"
#include "Initializers.h"
class GameObject;
class ResourceManager;

class Component
{
public:
	
	Component(GameObject* owner, ResourceManager* devices);
	Component();
	virtual ~Component() {}
	
	virtual bool initialize(ObjectFactoryPresets& presets);
	virtual std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects)=0;
	virtual std::unique_ptr<Component> copyMe() const = 0;
	//TODO::try and eliminate getDevices
	ResourceManager* getDevices() const { return devices; }
	GameObject* getOwner() const {return(_owner);}
	

protected:
	GameObject* _owner{ nullptr };
	ResourceManager* devices{ nullptr };
};


#endif