#include "GhostComponent.h"

GhostComponent::GhostComponent(std::unique_ptr<GameObject> owner):Component(owner){}

GhostComponent::~GhostComponent(){}

bool GhostComponent::initialize(ObjectFactoryPresets& presets)
{
	ghostDirection = presets.gDirection;
	return true;
}

void GhostComponent::Start(){}
std::unique_ptr<GameObject> GhostComponent::update(){return nullptr;}
void GhostComponent::Finish(){}