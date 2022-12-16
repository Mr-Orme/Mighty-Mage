#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Component.h"



class GameObject;

class InventoryComponent : public Component
{
public:
	//TODO:: this should be a type of trigger
	InventoryComponent(GameObject* owner, ResourceManager* devices);
	InventoryComponent() {}
	~InventoryComponent();

	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	std::unique_ptr<Component> copyMe() const override;


	Vector2D locationInPack() const {return packPosition;}
	void placeInPack(Vector2D packPosition){this -> packPosition = packPosition;}
protected:
	Vector2D packPosition{ 0,0 };
};

#endif