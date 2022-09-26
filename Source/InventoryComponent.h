#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Component.h"



class GameObject;

class InventoryComponent : public Component
{
public:
	InventoryComponent(GameObject* owner, ResourceManager* devices);
	~InventoryComponent();

	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;


	Vector2D locationInPack() const {return packPosition;}
	void placeInPack(Vector2D packPosition){this -> packPosition = packPosition;}
protected:
	Vector2D packPosition{ 0,0 };
};

#endif