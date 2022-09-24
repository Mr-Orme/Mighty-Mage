#ifndef BACKPACK_H
#define BACKPACK_H

#include <vector>
#include <array>
#include <memory>
#include "Component.h"
#include "Definitions.h"
class ResourceManager;
class GameObject;

class BackpackComponent : public Component
{
public:
	BackpackComponent(GameObject* owner, ResourceManager* devices);
	~BackpackComponent();

	bool initialize(ObjectFactoryPresets& presets) override;

	bool pickUpItem(GameObject* item);
	//TODO::Remove this
	//std::vector<std::unique_ptr<GameObject>>& GetInventory(){return inventory;}

	void openPack(bool open){this -> open = open;}
	bool isOpen(){return open;}

	
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	
protected:
	bool ToBackpack(std::unique_ptr<GameObject> item);
	std::vector<std::unique_ptr<GameObject>> inventory;
	std::vector<std::vector<bool> > openSlots;
	GameObject* pickedUpItem{ nullptr };
	int slotSize{ 0 };
	Vector2D topLeft{ 0,0 };
	Vector2D bottomRight{ 0,0 };
	int maxRows{ 0 };
	int maxColumns{ 0 };
	bool open{ false };
};

#endif