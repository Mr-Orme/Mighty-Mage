#ifndef BODY_H
#define BODY_H

#include "Component.h"
#include "Definitions.h"
class ResourceManager;
class GameObject;
class BodyComponent : public Component
{
public:
	BodyComponent(GameObject* owner);
	~BodyComponent();
	
	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);
	void Start();
	GameObject* Update();
	void Finish();

	GAME_FLT getAngle();
	GAME_VEC getCurrentSquare();
	GAME_VEC getPosition();
	GAME_VEC getVelocity();
	GAME_INT getWidth();
	GAME_INT getHeight();
	void setAngle(GAME_FLT angle);
	void adjustAngle(GAME_FLT adjustAmount);
	void linearStop();
	
protected:
	ResourceManager* devices;
	

};

#endif