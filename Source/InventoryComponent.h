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

	bool GetPickedUp(){return pickedUp;}
	GAME_VEC GetPackPosition(){return packPosition;}
	
	void SetPickedUp(bool pickedUp){this -> pickedUp = pickedUp;}
	void SetPackPosition(GAME_VEC packPosition){this -> packPosition = packPosition;}

	void Start();
	GameObject* Update();
	void Finish();
protected:
	bool pickedUp;
	GAME_VEC packPosition;
};

#endif