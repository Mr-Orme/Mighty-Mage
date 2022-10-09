#include "BodyComponent.h"
#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h"
#include "GraphicsDevice.h"
#include "Vector2D.h"
#include "Definitions.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"
#include "View.h"

BodyComponent::BodyComponent(GameObject* owner, ResourceManager* devices):Component(owner, devices){}
BodyComponent::BodyComponent():Component()
{
}
BodyComponent::~BodyComponent()
{
	//remove the physics body
	if (_owner && !devices->getPhysicsDevice()->RemoveObject(_owner))
	{
		printf("Object could not be removed from Physics World");
		//exit(1);
	}
}

//**************************************
//Based on the presets struct passed in, a fixture is created
bool BodyComponent::initialize(ObjectFactoryPresets& presets)
//**************************************
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Bodycomponent";
		return false;
	}
	if (auto sprite{ _owner->getComponent<SpriteComponent>() }; sprite != nullptr)
	{
		presets.bodyInitializers.dimensions = sprite->getTexture()->getDimensions();
	}
	if (presets.bodyInitializers.dimensions != Vector2D{ 0, 0 })
	{
		devices->getPhysicsDevice()->createFixture(_owner, presets.bodyInitializers);
	}
	return true;
}

//**************************************
//finds the current position, subtract's off the last frame's position
//to get the change in position. This is for the auto scrolling feature.
//the newpostion then becomes the old position.
std::unique_ptr<GameObject> BodyComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
//**************************************
{
	//*************************************** True to 90**********************
	//sometimes the angle get's off perpendicular.
	int angle = (int(devices->getPhysicsDevice()->getAngle(_owner)));

	if (angle > 360 && angle < 460) angle = 90;
	if (angle < 0) angle = 270;
	if (angle < 45 && angle >= 315) devices->getPhysicsDevice()->SetAngle(_owner, 0);
	else if (angle < 135 && angle >= 45) devices->getPhysicsDevice()->SetAngle(_owner, 90);
	else if (angle < 225 && angle >= 135) devices->getPhysicsDevice()->SetAngle(_owner, 180);
	else if (angle < 315 && angle >= 225) devices->getPhysicsDevice()->SetAngle(_owner, 270);

	return nullptr;
}

std::unique_ptr<Component> BodyComponent::copyMe() const
{
	return std::make_unique<BodyComponent>(*this);
}

void BodyComponent::turnLeft()
{
	devices->getPhysicsDevice()->SetAngle(_owner, devices->getPhysicsDevice()->getAngle(_owner) - angleAdjust);
	devices->getPhysicsDevice()->setLinearVelocity(_owner, { 0,0 });
}

void BodyComponent::turnRight()
{
	//set angle by angle adjust
	devices->getPhysicsDevice()->SetAngle(
		_owner,
		devices->getPhysicsDevice()->getAngle(_owner) + angleAdjust);
	//stop forward motion
	devices->getPhysicsDevice()->setLinearVelocity(_owner, { 0,0 });
}

void BodyComponent::moveForward()
{
	
	linearMovement(Way::forward);
}

void BodyComponent::moveBackward()
{
	linearMovement(Way::backward);

}

void BodyComponent::stop()
{
	devices->getPhysicsDevice()->setLinearVelocity(_owner, { 0,0 });
}

float BodyComponent::getAngle() const
{
	return devices->getPhysicsDevice()->getAngle(_owner);
}

Direction BodyComponent::getDirection() const
{
	return static_cast<Direction>(abs((int(getAngle()) % 360)));
}

Vector2D BodyComponent::getPosition() const
{
	return devices->getPhysicsDevice()->GetPosition(_owner);
}

void BodyComponent::turnOffPhysics() const
{
	devices->getPhysicsDevice()->SetStopPhysics(_owner);
}


Vector2D BodyComponent::getDimensions() const
{
	return _owner->getComponent<SpriteComponent>()->getTexture()->getDimensions();
}

Vector2D BodyComponent::getCenter() const
{
	auto position{ getPosition() };
	auto dimensions{ getDimensions() };
	return {(int) (position.x + dimensions.x / 2.0f), (int)(position.y + dimensions.y / 2.0f) };
}

void BodyComponent::linearMovement(Way direction)
{
	Vector2D applyForce{};
	std::string walkSound{ "walking" };
	if (devices->getInputDevice()->isPressed(Inputs::shift))
	{
		forceMultiplier = baseForceMultiplier * runMultiplier;
		walkSound = "run";
	}
	else
	{
		//back to walk speeds
		forceMultiplier = baseForceMultiplier;

	}
	//grab current velocity
	Vector2D velocity = devices->getPhysicsDevice()->GetVelocity(_owner);
	//Calculate force vector for a forward push
	
	applyForce.x = (int)cosf((devices->getPhysicsDevice()->getAngle(_owner) * Pi / 180) + direction*(Pi / 2)) * forceMultiplier;
	applyForce.y = (int)sinf((devices->getPhysicsDevice()->getAngle(_owner) * Pi / 180) + direction*(Pi / 2)) * forceMultiplier;
	
	//PUSH BABY PUSH!!!
	devices->getPhysicsDevice()->setLinearVelocity(_owner, applyForce);
	//if last play is done and the player is still moving
	if (abs(velocity.x) > 1 || abs(velocity.y) > 1)
	{
		//play walking sound
		devices->getSoundDevice()->PlaySound(SoundEffect::Event::walk, 0, 1);
	}
}
