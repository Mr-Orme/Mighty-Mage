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

	bool initialize(ObjectFactoryPresets& presets);

	bool GetPickedUp(){return pickedUp;}
	Vector2D GetPackPosition(){return packPosition;}
	
	void SetPickedUp(bool pickedUp){this -> pickedUp = pickedUp;}
	void SetPackPosition(Vector2D packPosition){this -> packPosition = packPosition;}

	void Start();
	std::unique_ptr<GameObject> update();
	void Finish();
protected:
	bool pickedUp{ false };
	Vector2D packPosition{ 0,0 };
};

#endif