#include "ComponentsList.h"
#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"


UserInputComponent::UserInputComponent(GameObject* owner, ResourceManager* devices):Component(owner, devices)
{
	for (int i = 0; i < (int)Event::numEvents; i++)
	{
		Event eventNum = static_cast<Event>(i);
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
		if (devices->GetInputDevice()->GetEvent(Event::up))
		{
			_owner->getComponent<BodyComponent>()->moveForward();

		}
		else if (devices->GetInputDevice()->GetEvent(Event::down))
		{
			_owner->getComponent<BodyComponent>()->moveBackward();

		}
		else
		{
			_owner->getComponent<BodyComponent>()->stop();
		}

		//Check for left or right buttons
		// the "turn" variable makes sure we only turn once every time we push the button
		if (devices->GetInputDevice()->GetEvent(Event::right))
		{
			//change the angle
			if (pressControl[Event::right])
			{
				_owner->getComponent<BodyComponent>()->turnRight();
				//ensures we only turn once per press
				pressControl[Event::right] = false;
			}
		}
		else pressControl[Event::right] = true;
		if (devices->GetInputDevice()->GetEvent(Event::left))
		{
			//similar to turn right.
			if (pressControl[Event::left])
			{
				_owner->getComponent<BodyComponent>()->turnLeft();
				pressControl[Event::left] = false;
			}
		}
		else pressControl[Event::left] = true;
	}
	else _owner->getComponent<BodyComponent>()->stop();

	if (devices->GetInputDevice()->GetEvent(Event::key_b))
	{
		if (pressControl[Event::key_b])
		{

			if (BackpackComponent* backpack{ _owner->getComponent<BackpackComponent>() };
				backpack != nullptr)
			{
				if (backpack->isOpen()) backpack->openPack(false);
				else backpack->openPack(true);
			}
			pressControl[Event::key_b] = false;
		}

	}
	else pressControl[Event::key_b] = true;


}

void UserInputComponent::displayLocation()
{
	Vector2D playerPosition = devices->GetPhysicsDevice()->GetPosition(_owner);
	std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
	devices->GetGraphicsDevice()->text2Screen(playerPositionText, { 10,10 });
}
