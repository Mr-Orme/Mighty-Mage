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
	BackpackComponent(GameObject* owner, ResourceManager* devices);
	~BackpackComponent();

	bool initialize(ObjectFactoryPresets& presets);

	bool AddItem(std::unique_ptr<GameObject> item);

	std::vector<std::unique_ptr<GameObject>>& GetInventory(){return inventory;}

	void SetOpen(bool open){this -> open = open;}
	bool GetOpen(){return open;}

	void Start();
	std::unique_ptr<GameObject> update();
	void Finish();
protected:
	bool ToBackpack(std::unique_ptr<GameObject> item);
	std::vector<std::unique_ptr<GameObject>> inventory;
	bool** backpack{ nullptr };
	
	int slotSize{ 0 };
	Vector2D topLeft{ 0,0 };
	Vector2D bottomRight{ 0,0 };
	int maxRows{ 0 };
	int maxColumns{ 0 };
	bool open{ false };
};

#endif