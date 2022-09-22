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
	UserInputComponent(GameObject* owner, ResourceManager* devices);
	~UserInputComponent();
	
	std::unique_ptr<GameObject> update() override;
	bool initialize(ObjectFactoryPresets& presets) override;
	
	void collidingWithWall(bool wallHit){this -> wallHit = wallHit;}

	

protected:
	Vector2D currentSquare();

	int frameCount{ 0 };
	//ObjectFactoryPresets presets;
	PhysicsStats physics;

	std::map<Event, bool> pressControl;
	bool wallHit{ false };
	bool noWall{ true };

	const int baseForceMultiplier{ 1500 }; //How fast does the player move.
	int forceMultiplier{ baseForceMultiplier };
	const int runMultiplier{ 3 }; //How many times faster is running then walking
	const int angleAdjust{ 90 }; //How many degrees does the player turn each time

	//Disntance from edge of screen before screen updates.	
	const int border{ 200 };
};

#endif