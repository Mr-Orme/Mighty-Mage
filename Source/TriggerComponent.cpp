#include "TriggerComponent.h"
#include "GameObject.h"

bool TriggerComponent::initialize(ObjectFactoryPresets& presets)
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Trigger component";
		return false;
	}
	direction = presets.triggerInitializers.direction;
	exitTo = presets.triggerInitializers.exitTo;
	message = presets.triggerInitializers.message;
	name = (Type)presets.triggerInitializers.name;
	return true;
}

std::unique_ptr<GameObject> TriggerComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	return std::unique_ptr<GameObject>();
}

std::unique_ptr<Component> TriggerComponent::copyMe() const
{
	return std::move(std::make_unique<TriggerComponent>(*this));
}
