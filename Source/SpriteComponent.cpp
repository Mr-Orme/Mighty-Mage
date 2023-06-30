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
SpriteComponent::SpriteComponent(GameObject* owner, ResourceManager* devices) :Component(owner, devices){}
SpriteComponent::SpriteComponent(ResourceManager* devices, std::string path) : 
	Component(),
	texture(std::make_shared<Texture>(path, devices->getGraphicsDevice()))
{

}
SpriteComponent::~SpriteComponent()
{
}

//**************************************
//on the first pass, we set up the texture for the object
bool SpriteComponent::initialize(ObjectFactoryPresets& presets)
//**************************************
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Sprite component";
		return false;
	}
	if(!initialized)
	{
		initialized = true;
	}
	return initialized;
}
std::unique_ptr<Component> SpriteComponent::copyMe() const
{
	return std::make_unique<SpriteComponent>(*this);
}
//**************************************
// Updates the position based on the view and draws the sprite
void SpriteComponent::run()
//**************************************
{
	BodyComponent* body{ _owner->getComponent<BodyComponent>() };
	run(
		devices->getGraphicsDevice()->getView()->relativePosition(body->getPosition()),
		body->getAngle());
}

void SpriteComponent::run(Vector2D position, float angle)
{
	texture ->draw(position, angle);
}

void SpriteComponent::changeSprite(std::shared_ptr<Texture> texture)
{
	{this->texture = texture; }
}



std::unique_ptr<GameObject> SpriteComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	run();
	return nullptr;
}



