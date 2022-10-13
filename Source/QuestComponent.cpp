#include <iostream>
#include "QuestComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
QuestComponent::QuestComponent(GameObject* owner, ResourceManager* devices) :
	Component(owner, devices)
{

}

bool QuestComponent::initialize(ObjectFactoryPresets& presets)
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Quest component";
		return false;
	}
	return true;
}

std::unique_ptr<GameObject> QuestComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	return nullptr;
}

std::unique_ptr<Component> QuestComponent::copyMe() const
{
	return std::move(std::make_unique<QuestComponent>(*this));
}