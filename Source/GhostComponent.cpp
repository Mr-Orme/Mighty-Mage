#include "GhostComponent.h"

GhostComponent::GhostComponent(GameObject* owner, ResourceManager* devices)
	:Component(owner, devices){}

GhostComponent::~GhostComponent(){}

bool GhostComponent::initialize(ObjectFactoryPresets& presets)
{
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


std::unique_ptr<GameObject> GhostComponent::update(){return nullptr;}
