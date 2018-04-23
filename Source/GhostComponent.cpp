#include "GhostComponent.h"

GhostComponent::GhostComponent(GameObject* owner):Component(owner){}

GhostComponent::~GhostComponent(){}

bool GhostComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
{
	ghostDirection = presets.gDirection;
	return true;
}

void GhostComponent::Start(){}
GameObject* GhostComponent::Update(){return nullptr;}
void GhostComponent::Finish(){}