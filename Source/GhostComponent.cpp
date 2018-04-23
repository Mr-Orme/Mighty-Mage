#include "GhostComponent.h"

GhostComponent::GhostComponent(std::shared_ptr<GameObject> owner):Component(owner){}

GhostComponent::~GhostComponent(){}

bool GhostComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
{
	ghostDirection = presets.gDirection;
	return true;
}

void GhostComponent::Start(){}
std::shared_ptr<GameObject> GhostComponent::Update(){return NULL;}
void GhostComponent::Finish(){}