#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include "GameObject.h"
#include "Definitions.h"


class Component
{
public:
	Component(std::shared_ptr<GameObject> owner);
	~Component();
	void OwnerDestroyed();

	virtual bool Initialize(GAME_OBJECTFACTORY_PRESETS& presets)=0;
	std::shared_ptr<GameObject> GetOwner();
	virtual void Start()=0;
	virtual std::shared_ptr<GameObject> Update()=0;
	virtual void Finish()=0;

protected:
	std::shared_ptr<GameObject> _owner;
};


#endif