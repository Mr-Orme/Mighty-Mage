#include "GameObject.h"
#include "Component.h"
#include "Initializers.h"
//**************************************
//owner is set on creation.
Component::Component(GameObject* owner, ResourceManager* devices) 
	: _owner(owner), devices(*devices)
{}

Component::Component(const Component& src)
	:_owner(src._owner), devices(src.devices)
{
}
Component::Component(Component&& src) noexcept
	:_owner(src._owner), devices(src.devices)
{
	src._owner = nullptr;
	
}
//**************************************



bool Component::initialize(ObjectFactoryPresets& presets)
{
	if (_owner == nullptr) _owner = presets.owner;
	
	return _owner != nullptr;
}



