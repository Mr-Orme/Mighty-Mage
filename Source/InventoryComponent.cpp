#include "InventoryComponent.h"
#include "GameObject.h"
InventoryComponent::InventoryComponent(GameObject* owner, ResourceManager* devices) :Component(owner, devices) {}

InventoryComponent::~InventoryComponent(){}

std::unique_ptr<GameObject> InventoryComponent::update(std::vector<std::unique_ptr<GameObject>>& objects){return nullptr;}

std::unique_ptr<Component> InventoryComponent::copyMe() const
{
	return std::make_unique<InventoryComponent>(*this);
}
