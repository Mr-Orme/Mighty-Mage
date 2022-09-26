#include "GameObject.h"
#include "Component.h"
//**************************************
//owner is set on creation.
Component::Component(GameObject* owner, ResourceManager* devices) 
	: _owner(owner), devices(devices)
{}
//**************************************

Component::~Component(){}

bool Component::initialize(ObjectFactoryPresets& presets)
{
	return true;
}



