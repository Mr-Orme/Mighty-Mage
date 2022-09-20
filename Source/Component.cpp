#include "GameObject.h"
#include "Component.h"
//**************************************
//owner is set on creation.
Component::Component(GameObject* owner, ResourceManager* devices) 
	: _owner(owner), devices(devices)
{}
//**************************************

Component::~Component(){}


GameObject* Component::getOwner()
{
	return(_owner);
}