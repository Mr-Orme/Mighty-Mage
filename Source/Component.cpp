#include "GameObject.h"
#include "Component.h"

Component::Component(GameObject* owner, ResourceManager* devices) 
	: _owner(owner), devices(devices)
{}
Component::Component(){}

bool Component::initialize(ObjectFactoryPresets& presets)
{
	if (_owner == nullptr) _owner = presets.owner;
	if (devices == nullptr) devices = presets.devices;
	return _owner != nullptr && devices != nullptr;
}



