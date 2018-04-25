#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Component.h"
#include "Definitions.h"

class GameObject;

class InventoryComponent : public Component
{
public:
	InventoryComponent(GameObject*);
	~InventoryComponent();

	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);

	bool isPickedUp();
	GAME_VEC GetPackPosition(){return packPosition;}
	GameObject* gotPickedUpBy() { return pickedUpBy; }
	
	void getPickedUp(GameObject* pickedUpBy);
	void SetPackPosition(GAME_VEC packPosition){this -> packPosition = packPosition;}

	void Start();
	GameObject* Update();
	void Finish();
protected:
	GAME_VEC packPosition;
	GameObject* pickedUpBy;
};

#endif