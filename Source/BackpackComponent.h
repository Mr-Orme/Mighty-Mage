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
	BackpackComponent(std::shared_ptr<GameObject>);
	~BackpackComponent();

	bool Initialize(GAME_OBJECTFACTORY_PRESETS& presets);

	bool AddItem(GameObject* item);

	std::vector<std::shared_ptr<GameObject>> GetInventory(){return inventory;}

	void SetOpen(bool open){this -> open = open;}
	bool GetOpen(){return open;}

	void Start();
	std::shared_ptr<GameObject> Update();
	void Finish();
protected:
	bool ToBackpack(std::shared_ptr<GameObject> item);
	std::vector<std::shared_ptr<GameObject>> inventory;
	bool** backpack;
	std::shared_ptr<ResourceManager> devices;
	
	GAME_INT slotSize;
	GAME_VEC topLeft;
	GAME_VEC bottomRight;
	GAME_INT maxRows;
	GAME_INT maxColumns;
	bool open;
};

#endif