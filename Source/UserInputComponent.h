//keymapping file
#ifndef USERINPUT_H
#define USERINPUT_H

#include "Component.h"
#include "Definitions.h"


class ResourceManager;
class ArtAssetLibrary;
class PhysicsAssetLibrary;
class ObjectFactory;

class UserInputComponent : public Component
{
public:
	UserInputComponent(std::shared_ptr<GameObject> owner);
	~UserInputComponent();
	
	bool Initialize(GAME_OBJECTFACTORY_PRESETS& presets);
	void SetWallHit(bool wallHit){this -> wallHit = wallHit;}

	GAME_VEC GetCurrentSquare();

	void Start();
	std::shared_ptr<GameObject> Update();
	void Finish();
protected:
	GAME_INT frameCount;
	std::shared_ptr<ResourceManager> devices;

	//GAME_OBJECTFACTORY_PRESETS presets;
	GAME_PHYSICS physics;
	GAME_VEC zeroVec;
	std::map<GAME_EVENT, bool> pressControl;
	bool wallHit;
	bool noWall;
	
};

#endif