#include "TriggerComponent.h"
#include "ComponentsList.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "GraphicsDevice.h"
TriggerComponent::TriggerComponent(GameObject* owner, ResourceManager* devices, TriggerPresets presets)
	:Component(owner, devices), 
	name((Type)presets.name),
	exitTo(presets.exitTo),
	message(presets.message),
	playerLocation(presets.playerLocation),
	playerDirection((Direction)presets.playerAngle)
{
}
bool TriggerComponent::initialize(ObjectFactoryPresets& presets)
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Trigger component";
		return false;
	}
	direction = (Direction)presets.bodyInitializers.angle;

	switch (direction)
	{
	case Direction::N:
		presets.bodyInitializers.dimensions = { devices.pixelsPerSquare, (int)(devices.pixelsPerSquare * 0.2f) };
		break;
	case Direction::E:
		presets.bodyInitializers.dimensions = { (int)(devices.pixelsPerSquare * 0.2f), devices.pixelsPerSquare };
		presets.bodyInitializers.position.x += devices.pixelsPerSquare - presets.bodyInitializers.dimensions.x;
		break;
	case Direction::S:
		presets.bodyInitializers.dimensions = { devices.pixelsPerSquare, (int)(devices.pixelsPerSquare * 0.2f) };
		presets.bodyInitializers.position.y += devices.pixelsPerSquare - presets.bodyInitializers.dimensions.y;
		break;
	case Direction::W:
		presets.bodyInitializers.dimensions = { (int)(devices.pixelsPerSquare * 0.2f), devices.pixelsPerSquare };
		break;
	default:
		break;
	}

	return true;
}

std::unique_ptr<GameObject> TriggerComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	if (triggered)
	{
		if (auto body{ triggerer->getComponent<BodyComponent>() }; body != nullptr)
		{
			body->stop();
		}
		switch (name)
		{
		case TriggerComponent::Type::exits:
			devices.getGraphicsDevice()->notice2Screen(message);
			if (devices.getInputDevice()->isPressed(Inputs::key_y))
			{
				devices.upause();
				triggered = false;
				devices.changeLevel(exitTo, playerLocation, playerDirection);
			}
			else if (devices.getInputDevice()->isPressed(Inputs::key_n))
			{
				devices.upause();
				triggered = false;
			}
			else return false;
			break;
		case TriggerComponent::Type::messages:
			break;
		case TriggerComponent::Type::battles:
			break;
		case TriggerComponent::Type::quest:
			break;
		}
	
	}
	return nullptr;
}

std::unique_ptr<Component> TriggerComponent::copyMe() const
{
	return std::move(std::make_unique<TriggerComponent>(*this));
}

bool TriggerComponent::trigger(Direction direction, GameObject* triggerer)
{
	
	switch (name)
	{
	case TriggerComponent::Type::exits:
		//devices.getGraphicsDevice()->text2Screen(std::to_string((int)direction), { 10, 50 });
		if (this->direction == direction)
		{
			triggered = true;
			devices.pause();
			this->triggerer = triggerer;
		}
		break;
	case TriggerComponent::Type::messages:
		break;
	case TriggerComponent::Type::battles:
		break;
	}
	return triggered;
}


