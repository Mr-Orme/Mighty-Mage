#include <iostream>

#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "ObjectLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "GraphicsDevice.h"
#include <memory>
SpriteComponent::SpriteComponent(ResourceManager* devices, std::string path) : 
	Component(),
	texture(std::make_shared<Texture>(path, devices->getGraphicsDevice()))
{}

SpriteComponent::~SpriteComponent(){}

bool SpriteComponent::initialize(ObjectFactoryPresets& presets)
{
	return Component::initialize(presets);
}
std::unique_ptr<Component> SpriteComponent::copyMe() const
{
	return std::make_unique<SpriteComponent>(*this);
}

void SpriteComponent::changeSprite(std::shared_ptr<Texture> texture)
{
	this->texture = texture;
}

std::unique_ptr<GameObject> SpriteComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	auto body{ _owner->getComponent<BodyComponent>() };
	texture->draw(devices->getGraphicsDevice()->getView()->relativePosition(body->getPosition()),
		body->getAngle());
	return nullptr;
}



