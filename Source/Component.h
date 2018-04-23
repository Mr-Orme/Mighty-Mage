#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include "GameObject.h"
#include "Definitions.h"

class ObjectFactory;
class Component
{
public:
	Component(GameObject* owner);
	~Component();
	void OwnerDestroyed();

	virtual bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)=0;
	GameObject* GetOwner();
	virtual void Start()=0;
	virtual GameObject* Update()=0;
	virtual void Finish()=0;

protected:
	GameObject* _owner;
};


#endif