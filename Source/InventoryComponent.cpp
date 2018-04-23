#include "InventoryComponent.h"

InventoryComponent::InventoryComponent(GameObject* owner):Component(owner){}

InventoryComponent::~InventoryComponent(){}

bool InventoryComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
{
	pickedUp = false;
	return true;
}

void InventoryComponent::Start(){}
GameObject* InventoryComponent::Update(){return nullptr;}
void InventoryComponent::Finish(){}