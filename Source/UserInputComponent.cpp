#include "ComponentsList.h"
#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"

using input = InputDevice::Inputs;
UserInputComponent::UserInputComponent(GameObject* owner, ResourceManager* devices):Component(owner, devices)
{
	for (int i = 0; i < (int)input::numEvents; i++)
	{
		input eventNum = static_cast<input>(i);
		pressControl[eventNum] = true;
	}
}

UserInputComponent::~UserInputComponent(){}




//**************************************
//reacts to keyboard input and adjusts the world accoringly.
std::unique_ptr<GameObject> UserInputComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
//**************************************
{
	
	dealWithButtonPresses();
	
	devices->GetGraphicsDevice()->getView()->borderDectection(
		_owner->getComponent<BodyComponent>()->getPosition());
	
	displayNotices();
	
	return nullptr;
}
void UserInputComponent::displayNotices()
{
	//this is the game square in the 15x15 map.
	BodyComponent* body{ _owner->getComponent<BodyComponent>() };
	

	//set up ntoice
	Notice notice{
		body->currentSquare(),
		body->getDirection(),
		"" };


	//if there is a notice
	notice = devices->GetNoticesLibrary()->search(notice);
	if (notice.text != "")
	{
		//display it.
		devices->GetGraphicsDevice()->notice2Screen(notice.text);
	}
	//*****************************TO Basement*********************************************
	//if we are on the main level and make it to the proper spot

	//TODO::make exiting a component!
	if (devices->isExitSquare(body->currentSquare()))
	{
		
		//TODO::make more generic to load any level!
		devices->SetLoadBasement(true);
	}
	//********************************************************************************
}
void UserInputComponent::dealWithButtonPresses()
{
	if (!_owner->getComponent<BackpackComponent>()->isOpen())
	{
		if (devices->GetInputDevice()->isPressed(input::up))
		{
			_owner->getComponent<BodyComponent>()->moveForward();

		}
		else if (devices->GetInputDevice()->isPressed(input::down))
		{
			_owner->getComponent<BodyComponent>()->moveBackward();

		}
		else
		{
			_owner->getComponent<BodyComponent>()->stop();
		}

		//Check for left or right buttons
		// the "turn" variable makes sure we only turn once every time we push the button
		if (devices->GetInputDevice()->isPressed(input::right))
		{
			//change the angle
			if (pressControl[input::right])
			{
				_owner->getComponent<BodyComponent>()->turnRight();
				//ensures we only turn once per press
				pressControl[input::right] = false;
			}
		}
		else pressControl[input::right] = true;
		if (devices->GetInputDevice()->isPressed(input::left))
		{
			//similar to turn right.
			if (pressControl[input::left])
			{
				_owner->getComponent<BodyComponent>()->turnLeft();
				pressControl[input::left] = false;
			}
		}
		else pressControl[input::left] = true;
	}
	else _owner->getComponent<BodyComponent>()->stop();

	if (devices->GetInputDevice()->isPressed(input::key_b))
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
	Vector2D playerPosition = devices->GetPhysicsDevice()->GetPosition(_owner);
	std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
	devices->GetGraphicsDevice()->text2Screen(playerPositionText, { 10,10 });
}
