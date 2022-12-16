#ifndef BACKPACK_H
#define BACKPACK_H

#include <vector>
#include <memory>

#include "Component.h"

#include "Initializers.h"

class GameObject;
class ResourceManager;
//class GameObject;

class BackpackComponent : public Component
{
public:
	BackpackComponent(GameObject* owner, ResourceManager* devices);
	BackpackComponent() = delete;
	BackpackComponent(const BackpackComponent& src);
	~BackpackComponent() = default;

	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	std::unique_ptr<Component> copyMe() const override;

	bool pickUpItem(GameObject* item);

	void openPack(bool open){this -> open = open;}
	bool isOpen() const {return open;}
	
protected:
	bool ToBackpack(std::unique_ptr<GameObject> item);
	void reset();
	std::vector<std::unique_ptr<GameObject> > inventory;
	std::vector<std::vector<bool> > openSlots;

	GameObject* pickedUpItem{ nullptr };
	const int slotSize{ 25 };//pixels
	Vector2D topLeft{ 0,0 };
	Vector2D bottomRight{ 0,0 };
	Vector2D max{ 0,0 };
	bool open{ false };

	RGBA background{ 255, 255, 255 , 220 };
	RGBA border{ 0,0,0,255 };
};

#endif