//keymapping file
#ifndef USERINPUT_H
#define USERINPUT_H

#include "Component.h"



class ResourceManager;
class ArtAssetLibrary;
class PhysicsAssetLibrary;
class ObjectFactory;
enum class Event { NA, up, down, left, right, space, shift, key_b, quit, numEvents };
class UserInputComponent : public Component
{
public:
	UserInputComponent(GameObject* owner, ResourceManager* devices);
	~UserInputComponent();
	
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	
	
	//TODO::This should not be here....
	void collidingWithWall(bool wallHit){this -> wallHit = wallHit;}

	

protected:
	void displayNotices();
	void dealWithButtonPresses();
	void displayLocation();
	int frameCount{ 0 };

	std::map<Event, bool> pressControl;
	bool wallHit{ false };
	bool noWall{ true };

	
};

#endif