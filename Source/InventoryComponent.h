#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Component.h"
#include "Definitions.h"

class GameObject;

class InventoryComponent : public Component
{
public:
	InventoryComponent(GameObject* owner, ResourceManager* devices);
	~InventoryComponent();

	std::unique_ptr<GameObject> update() override;
	bool initialize(ObjectFactoryPresets& presets) override;

	bool isPickedUp() const {return pickedUp;}
	Vector2D locationInPack() const {return packPosition;}
	
	void pickUp(){pickedUp = true;}
	void placeInPack(Vector2D packPosition){this -> packPosition = packPosition;}


	

protected:
	bool pickedUp{ false };
	Vector2D packPosition{ 0,0 };
};

#endif