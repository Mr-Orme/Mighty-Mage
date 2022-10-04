#include "TriggerComponent.h"

bool TriggerComponent::initialize(ObjectFactoryPresets& presets)
{
	return true;
}

std::unique_ptr<GameObject> TriggerComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	return std::unique_ptr<GameObject>();
}

std::unique_ptr<Component> TriggerComponent::copyMe() const
{
	return std::unique_ptr<Component>();
}
