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
	UserInputComponent(std::unique_ptr<GameObject> owner);
	~UserInputComponent();
	
	bool initialize(ObjectFactoryPresets& presets);
	void SetWallHit(bool wallHit){this -> wallHit = wallHit;}

	Vector2D GetCurrentSquare();

	void Start();
	std::unique_ptr<GameObject> update();
	void Finish();
protected:
	int frameCount;
	std::unique_ptr<ResourceManager> devices;

	//ObjectFactoryPresets presets;
	PhysicsStats physics;
	Vector2D zeroVec;
	std::map<Event, bool> pressControl;
	bool wallHit;
	bool noWall;
	
};

#endif