//keymapping file
#ifndef USERINPUT_H
#define USERINPUT_H

#include "Component.h"
#include "InputDevice.h"


class ResourceManager;
class ArtAssetLibrary;
class PhysicsAssetLibrary;
class ObjectFactory;

class UserInputComponent : public Component
{
public:
	UserInputComponent(GameObject* owner, ResourceManager* devices);
	UserInputComponent() = delete;
	~UserInputComponent();
	
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;	
	std::unique_ptr<Component> copyMe() const override;

protected:
	void displayNotices();
	void dealWithButtonPresses();
	void displayLocation();
	int frameCount{ 0 };

	std::map<Inputs, bool> pressControl;


	
};

#endif