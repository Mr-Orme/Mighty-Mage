#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <vector>

#include "Vector2D.h"

struct ObjectFactoryPresets;
class GameObject;
class ResourceManager;

class Component
{
public:
	
	Component(GameObject* owner, ResourceManager* devices);
	Component();
	Component(const Component& src);
	Component(Component&& src) noexcept;
	virtual ~Component() {}
	
	virtual bool initialize(ObjectFactoryPresets& presets);
	virtual std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects)=0;
	virtual std::unique_ptr<Component> copyMe() const = 0;

	GameObject* getOwner() const {return(_owner);}
	

protected:
	GameObject* _owner{ nullptr };
	ResourceManager* devices{ nullptr };//TODO::Make a reference!
};


#endif