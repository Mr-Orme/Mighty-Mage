#include "InventoryComponent.h"
#include "GameObject.h"
InventoryComponent::InventoryComponent(GameObject* owner, ResourceManager* devices) :Component(owner, devices) {}

InventoryComponent::~InventoryComponent(){}

bool InventoryComponent::initialize(ObjectFactoryPresets& presets)
{
	pickedUp = false;
	return true;
}


std::unique_ptr<GameObject> InventoryComponent::update(){return nullptr;}
