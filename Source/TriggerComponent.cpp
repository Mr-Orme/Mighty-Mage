#include "TriggerComponent.h"
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
	return true;
}

std::unique_ptr<GameObject> TriggerComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	return std::unique_ptr<GameObject>();
}

std::unique_ptr<Component> TriggerComponent::copyMe() const
{
	return std::move(std::make_unique<TriggerComponent>(*this));
}

bool TriggerComponent::trigger(Direction direction)
{
	switch (name)
	{
	case TriggerComponent::Type::exits:
		devices->getGraphicsDevice()->text2Screen(std::to_string((int)direction), { 10, 50 });
		if (this->direction == direction)
		{
			devices->getGraphicsDevice()->notice2Screen(message);
			bool y{ devices->getInputDevice()->isPressed(InputDevice::Inputs::key_y) };
			bool n{ devices->getInputDevice()->isPressed(InputDevice::Inputs::key_n) };
			while (!y || !n)
			{
				devices->getGraphicsDevice()->present();
				y= devices->getInputDevice()->isPressed(InputDevice::Inputs::key_y);
				n= devices->getInputDevice()->isPressed(InputDevice::Inputs::key_n);
			}
			if (y)
				devices->changeLevel(exitTo);
			
		}
		else return false;
		break;
	case TriggerComponent::Type::messages:
		break;
	case TriggerComponent::Type::battles:
		break;
	}
	return true;
}
