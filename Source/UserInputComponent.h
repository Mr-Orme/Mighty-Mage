//keymapping file
#ifndef USERINPUT_H
#define USERINPUT_H

#include "Component.h"
#include "Definitions.h"
#include "InputDevice.h"
#include "PhysicsDevice.h"

class ResourceManager;
class ArtAssetLibrary;
class PhysicsAssetLibrary;
class ObjectFactory;
struct node
{
	GAME_DIRECTION direction;
	node* next;
	node*prev;
};

class UserInputComponent : public Component
{
public:
	

	UserInputComponent(GameObject* owner);
	~UserInputComponent();
	
	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);
	void SetWallHit(bool wallHit){this -> wallHit = wallHit;}

	void Start() override;
	GameObject* Update() override;
	void Finish() override;
protected:

	node* current;
	//GAME_INT frameCount;
	ResourceManager* devices;

	//GAME_OBJECTFACTORY_PRESETS presets;
	PhysicsDevice::GAME_PHYSICS physics;
	std::map<InputDevice::GAME_EVENT, bool> pressControl;
	bool wallHit;
	bool linearMovement;
	bool wallSoundPlayed;
	
	const GAME_INT BASE_FORCE_MULTIPLIER;
	const GAME_INT RUN_MULTIPLIER; //How many times faster is running then walking
	
};

#endif