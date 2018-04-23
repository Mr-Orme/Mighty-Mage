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


class UserInputComponent : public Component
{
public:
	

	UserInputComponent(std::shared_ptr<GameObject> owner);
	~UserInputComponent();
	
	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);
	void SetWallHit(bool wallHit){this -> wallHit = wallHit;}

	GAME_VEC GetCurrentSquare();

	void Start();
	std::shared_ptr<GameObject> Update();
	void Finish();
protected:
	//GAME_INT frameCount;
	ResourceManager* devices;

	//GAME_OBJECTFACTORY_PRESETS presets;
	PhysicsDevice::GAME_PHYSICS physics;
	std::map<InputDevice::GAME_EVENT, bool> pressControl;
	bool wallHit;
	bool noWallSound;
	
};

#endif