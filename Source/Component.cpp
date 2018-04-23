#include "GameObject.h"
#include "Component.h"
//**************************************
//owner is set on creation.
Component::Component(GameObject* owner) : _owner(owner){}
//**************************************

Component::~Component(){}

//**************************************
//when the owner is destroyed, run the finish for the component
void Component::OwnerDestroyed()
//**************************************
{
	Finish();
	_owner = nullptr;
}

GameObject* Component::GetOwner()
{
	return(_owner);
}