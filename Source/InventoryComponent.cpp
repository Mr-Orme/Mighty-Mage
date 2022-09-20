#include "InventoryComponent.h"

InventoryComponent::InventoryComponent(std::unique_ptr<GameObject> owner):Component(owner){}

InventoryComponent::~InventoryComponent(){}

bool InventoryComponent::initialize(ObjectFactoryPresets& presets)
{
	pickedUp = false;
	return true;
}

void InventoryComponent::Start(){}
std::unique_ptr<GameObject> InventoryComponent::update(){return nullptr;}
void InventoryComponent::Finish(){}