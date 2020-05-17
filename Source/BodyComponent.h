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
	
	bool Initialize(GAME_OBJECTFACTORY_PRESETS& presets);
	void Start();
	std::shared_ptr<GameObject> Update();
	void Finish();

	GAME_FLT GetAngle();
	std::shared_ptr<ResourceManager> GetDevices(){return devices;}
	GAME_VEC GetDPosition(){return dPosition;}
	
protected:
	std::shared_ptr<ResourceManager> devices;
	GAME_VEC dPosition;
	GAME_VEC oldPosition;
};

#endif