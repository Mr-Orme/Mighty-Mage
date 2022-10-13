#include "ComponentsList.h"
#include "ResourceManager.h"
#include "GraphicsDevice.h"
#include "GameObject.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"
#include "NoticesAssetLibrary.h"
#include "View.h"
//TODO::Get rid of
#include <string>
#include<sstream>

using input = Inputs;
UserInputComponent::UserInputComponent(GameObject* owner, ResourceManager* devices):Component(owner, devices)
{
	for (int i = 0; i < (int)input::numEvents; i++)
	{
		input eventNum = static_cast<input>(i);
		pressControl[eventNum] = true;
	}
}

UserInputComponent::~UserInputComponent(){}

std::unique_ptr<GameObject> UserInputComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	//TODO::Get rid of********************
	auto center{ _owner->getComponent<BodyComponent>()->getPosition()};
	auto square{ devices->pixel2Square(center) };
	std::stringstream output;
	output << "Player: " << center.x << ", " << center.y << "\n"
		<< square.x << ", " << square.y;
	devices->getGraphicsDevice()->text2Screen(output.str(), { 10,10 });
	auto viewP{ devices->getGraphicsDevice()->getView()->getViewingWindowPosition() };
	output=std::stringstream();
	output << "View:" << viewP.x << ", " << viewP.y;
	devices->getGraphicsDevice()->text2Screen(output.str(), { 10,40 });

	//*************************************************
	if(!devices->isPaused())
		dealWithButtonPresses();
	
	devices->getGraphicsDevice()->getView()->borderDectection(
		_owner->getComponent<BodyComponent>()->getPosition());
	
	displayNotices();
	
	return nullptr;
}
std::unique_ptr<Component> UserInputComponent::copyMe() const
{
	return std::make_unique<UserInputComponent>(*this);
}
void UserInputComponent::displayNotices()
{
	BodyComponent* body{ _owner->getComponent<BodyComponent>() };
	
	if (
		Notice notice{ devices->pixel2Square(body->getPosition()), body->getDirection(), "" };
		devices->getNoticesLibrary()->search(notice)
		)
	{
		devices->getGraphicsDevice()->notice2Screen(notice.text);
	}
}
void UserInputComponent::dealWithButtonPresses()
{
	if (auto pack{ _owner->getComponent<BackpackComponent>() }; pack && !pack->isOpen())
	{
		if (devices->getInputDevice()->isPressed(input::up))
		{
			_owner->getComponent<BodyComponent>()->moveForward();
		}
		else if (devices->getInputDevice()->isPressed(input::down))
		{
			_owner->getComponent<BodyComponent>()->moveBackward();
		}
		else
		{
			_owner->getComponent<BodyComponent>()->stop();
			devices->getSoundDevice()->stopSounds();
		}

		// the "pressControl" variable makes sure we only turn once every time we push the button
		if (devices->getInputDevice()->isPressed(input::right))
		{
			if (pressControl[input::right])
			{
				_owner->getComponent<BodyComponent>()->turnRight();
		
				pressControl[input::right] = false;
			}
		}
		else pressControl[input::right] = true;
		if (devices->getInputDevice()->isPressed(input::left))
		{
			if (pressControl[input::left])
			{
				_owner->getComponent<BodyComponent>()->turnLeft();
				pressControl[input::left] = false;
			}
		}
		else pressControl[input::left] = true;
	}
	else _owner->getComponent<BodyComponent>()->stop();

	if (devices->getInputDevice()->isPressed(input::key_b))
	{
		if (pressControl[input::key_b])
		{

			if (BackpackComponent* backpack{ _owner->getComponent<BackpackComponent>() };
				backpack != nullptr)
			{
				if (backpack->isOpen()) backpack->openPack(false);
				else backpack->openPack(true);
			}
			pressControl[input::key_b] = false;
		}

	}
	else pressControl[input::key_b] = true;


}

void UserInputComponent::displayLocation()
{
	Vector2D playerPosition = devices->getPhysicsDevice()->GetPosition(_owner);
	std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
	devices->getGraphicsDevice()->text2Screen(playerPositionText, { 10,10 });
}
