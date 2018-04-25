#ifndef BACKPACK_H
#define BACKPACK_H

#include <vector>
#include "Component.h"
#include "Definitions.h"
class ResourceManager;
class GameObject;

class BackpackComponent : public Component
{
public:
	BackpackComponent(GameObject*);
	~BackpackComponent();

	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);

	bool AddItem(std::unique_ptr<GameObject> item);

	std::vector<std::unique_ptr<GameObject>>& GetInventory(){return inventory;}

	void SetOpen(bool open){this -> open = open;}
	bool GetOpen(){return open;}

	void Start();
	GameObject* Update();
	void Finish();
protected:
	std::vector<std::unique_ptr<GameObject>> inventory;
	bool** backpack;
	ResourceManager* devices;
	
	GAME_INT slotSize;
	GAME_VEC topLeft;
	GAME_VEC bottomRight;
	GAME_VEC numSlots;
	bool open;
};

#endif