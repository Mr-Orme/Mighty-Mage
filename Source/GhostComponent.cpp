#include <memory>
#include <vector>

#include "GhostComponent.h"
#include "Definitions.h"
#include "GameObject.h"
#include "ResourceManager.h"
GhostComponent::GhostComponent(GameObject* owner, ResourceManager* devices)
	:Component(owner, devices){}

GhostComponent::GhostComponent()
{
}

GhostComponent::~GhostComponent(){}

bool GhostComponent::initialize(ObjectFactoryPresets& presets)
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Ghost component";
		return false;
	}
	ghostDirection = presets.gDirection;
	return true;
}

bool GhostComponent::canPass(Direction direction) const
{
	if (const auto canGo{ ghostDirection.find(direction) }; canGo != ghostDirection.end())
	{
		return canGo->second;
	}
	return false;
}


std::unique_ptr<GameObject> GhostComponent::update(std::vector<std::unique_ptr<GameObject>>& objects){return nullptr;}

std::unique_ptr<Component> GhostComponent::copyMe() const
{
	return std::make_unique<GhostComponent>(*this);
}
