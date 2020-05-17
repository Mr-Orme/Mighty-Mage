#include "InventoryComponent.h"

InventoryComponent::InventoryComponent(std::shared_ptr<GameObject> owner):Component(owner){}

InventoryComponent::~InventoryComponent(){}

bool InventoryComponent::Initialize(GAME_OBJECTFACTORY_PRESETS& presets)
{
	pickedUp = false;
	return true;
}

void InventoryComponent::Start(){}
std::shared_ptr<GameObject> InventoryComponent::Update(){return NULL;}
void InventoryComponent::Finish(){}