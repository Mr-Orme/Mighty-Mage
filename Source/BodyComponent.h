#ifndef BODY_H
#define BODY_H

#include "Component.h"
#include "Definitions.h"
class ResourceManager;

class BodyComponent : public Component
{
public:
	BodyComponent(std::shared_ptr<GameObject> owner);
	~BodyComponent();
	
	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);
	void Start();
	std::shared_ptr<GameObject> Update();
	void Finish();

	GAME_FLT GetAngle();
	ResourceManager* GetDevices(){return devices;}
	GAME_VEC GetDPosition(){return dPosition;}
	
protected:
	ResourceManager* devices;
	GAME_VEC dPosition;
	GAME_VEC oldPosition;
};

#endif