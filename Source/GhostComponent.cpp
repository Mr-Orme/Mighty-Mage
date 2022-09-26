#include "GhostComponent.h"
#include "GameObject.h"
GhostComponent::GhostComponent(GameObject* owner, ResourceManager* devices)
	:Component(owner, devices){}

GhostComponent::~GhostComponent(){}

bool GhostComponent::initialize(ObjectFactory::Presets& presets)
{
	ghostDirection = presets.gDirection;
	return true;
}

bool GhostComponent::canPass(BodyComponent::Direction direction) const
{
	if (const auto canGo{ ghostDirection.find(direction) }; canGo != ghostDirection.end())
	{
		return canGo->second;
	}
	return false;
}


std::unique_ptr<GameObject> GhostComponent::update(std::vector<std::unique_ptr<GameObject>>& objects){return nullptr;}
