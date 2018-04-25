#include "InventoryComponent.h"

InventoryComponent::InventoryComponent(GameObject* owner):Component(owner){}

InventoryComponent::~InventoryComponent(){}

bool InventoryComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
{
	pickedUpBy = nullptr;
	return true;
}

bool InventoryComponent::isPickedUp()
{
	if (pickedUpBy == nullptr)
	{
		return false;
	}
	return true;
}

void InventoryComponent::getPickedUp(GameObject * pickedUpBy)
{
	this->pickedUpBy = pickedUpBy;
}

void InventoryComponent::Start(){}
GameObject* InventoryComponent::Update(){return nullptr;}
void InventoryComponent::Finish(){}