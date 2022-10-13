#include "TriggerComponent.h"
#include "ComponentsList.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "GraphicsDevice.h"
bool TriggerComponent::initialize(ObjectFactoryPresets& presets)
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Trigger component";
		return false;
	}
	direction = (Direction)presets.bodyInitializers.angle;
	exitTo = presets.triggerInitializers.exitTo;
	message = presets.triggerInitializers.message;
	name = (Type)presets.triggerInitializers.name;
	playerLocation = presets.triggerInitializers.playerLocation;
	playerDirection = (Direction)presets.triggerInitializers.playerAngle;
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
			devices->getGraphicsDevice()->notice2Screen(message);
			if (devices->getInputDevice()->isPressed(Inputs::key_y))
			{
				devices->upause();
				triggered = false;
				devices->changeLevel(exitTo, playerLocation, playerDirection);
			}
			else if (devices->getInputDevice()->isPressed(Inputs::key_n))
			{
				devices->upause();
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
		//devices->getGraphicsDevice()->text2Screen(std::to_string((int)direction), { 10, 50 });
		if (this->direction == direction)
		{
			triggered = true;
			devices->pause();
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


