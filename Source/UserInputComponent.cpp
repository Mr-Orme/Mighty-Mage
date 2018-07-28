#include "ComponentsList.h"
#include "DeviceList.h"

#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "AssetLibrary.h"



UserInputComponent::UserInputComponent(GameObject* owner):Component(owner), BASE_FORCE_MULTIPLIER(1500), RUN_MULTIPLIER(3){}
UserInputComponent::~UserInputComponent(){}

//**************************************
//Set up some defaults
bool UserInputComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
{
	
	devices = presets.devices;
	wallHit = false;
	linearMovement =false;
	wallSoundPlayed = false;

	for (int i = 0; i < InputDevice::GAME_NUM_EVENTS; i++)
	{
		pressControl[(InputDevice::GAME_EVENT)i] = true;
	}

	node* head = new node;
	
	current = head;
	current->prev = nullptr;
	current->direction = N;
	
	current->next = new node;
	current->next->prev = current;
	current->next->direction = E;
	
	current->next->next = new node;
	current->next->next->prev = current->next;
	current->next->next->direction = S;

	current->next->next->next = new node;
	current->next->next->next->prev = current->next->next;
	current->next->next->next->direction = W;

	//make it circular!
	current->next->next->next->next = head;
	head->prev = current->next->next->next;

	current = head;

	//use owner's position to scroll when boder approached!
	devices->getGraphicsDevice()->GetView()->addScroller(_owner);
	return true;
}
	
void UserInputComponent::Start(){}

//**************************************
//reacts to keyboard input and adjusts the world accoringly.
GameObject* UserInputComponent::Update()
//**************************************
{
	//HACK: adjust walking sounds so that there is dead space after sound so that framecount is unnecessary
	static int frameCount = 0;
	
	if(!_owner -> GetComponent<BackpackComponent>() -> GetOpen())
	{
		GAME_INT soundWait = devices->getFPS() / 5;//adjusts how long between playing of sound effects.
		const std::string walkSound = "walking";
		const std::string runSound = "run";
		std::string sound = walkSound;

		 //How fast does the player move.
		GAME_INT forceMultiplier = BASE_FORCE_MULTIPLIER;
		

		//*****************Adjustmnts for running************************
		if (devices->getInputDevice()->GetEvent(InputDevice::GAME_SHIFT))
		{
			forceMultiplier = BASE_FORCE_MULTIPLIER * RUN_MULTIPLIER;
			soundWait = devices->getFPS() / 8;
			sound = runSound;
		}
		else
		//back to walking
		{
			soundWait = devices->getFPS() / 5;
			forceMultiplier = BASE_FORCE_MULTIPLIER;
			sound = walkSound;
		}
		//*****************************************************************
		//TODO: stop walking sound when button let go of!
		//*******************sound check************
		if (frameCount > soundWait + 1)//walk sound every so often
		{
			devices->getSoundDevice()->PlaySound(sound, 0, 1);
			frameCount = 0;
		}
		//TODO: no sound if up/down held, press both, let go of the first... use press control vector!
		/*	1. Collision with wall && (wallHit)
			2. Forward or back button pressed && (linearMovement)
			3. only once per press (wallSoundPlayed)
			4. reset on right/left turn (wallSoundPlayed=false & linearMovement=false [this one so it doesn't play immediatly])
			5. velocity check to make sure we've started moving again!
			*/
		//wall sound based on forward/backward motion and collision detection
		if (!wallSoundPlayed && linearMovement && wallHit && abs(_owner->GetComponent<BodyComponent>()->getVelocity()) < 1)
		{
			devices->getSoundDevice()->PlaySound("wall", 0, 2);
			wallHit = false;
			wallSoundPlayed = true;
		}
		//*****************************************

		//**************Up or Down or neither************
		int forceDirection = 0;
		

		if (devices->getInputDevice()->GetEvent(InputDevice::GAME_UP))
		{
			forceDirection = -1;
			linearMovement = true;
		}

		else if (devices->getInputDevice()->GetEvent(InputDevice::GAME_DOWN))
		{
			forceDirection = 1;
			linearMovement = true;
		}

		else
		{
			linearMovement = false;
			wallSoundPlayed = false;
		}
		//***********************************************

		//****************MOVE!*****************
		if (forceDirection != 0)
		{
			//PUSH BABY PUSH!!!
			devices->getPhysicsDevice()->SetLinearVelocity
			(
				_owner,
				//x=(cos(angle) + or - (PI/2)) * (force)
				//y=(sin(angle) + or - (PI/2)) * (force)
				{ 
					(float)cosf((_owner->GetComponent<BodyComponent>()->getAngle()*PI / 180) + forceDirection * (PI / 2))*forceMultiplier,
					(float)sinf((_owner->GetComponent<BodyComponent>()->getAngle()*PI / 180) + forceDirection * (PI / 2))*forceMultiplier
				}
			);
			frameCount++;
		}
		//Don't move!
		else
		{
			_owner->GetComponent<BodyComponent>()->linearStop();
			frameCount = 0;
			
		}
		//****************************************
		
		
		
		//****************left or right*************
		if(devices -> getInputDevice() -> GetEvent(InputDevice::GAME_RIGHT))
		{
			if(pressControl[InputDevice::GAME_RIGHT])
			{
				current = current->next;
				_owner->GetComponent<BodyComponent>()->linearStop();

				linearMovement = false;
				wallSoundPlayed = false;
				
				pressControl[InputDevice::GAME_RIGHT] = false;
			}
		}
		else pressControl[InputDevice::GAME_RIGHT] = true;

		if(devices -> getInputDevice() -> GetEvent(InputDevice::GAME_LEFT))
		{
			if(pressControl[InputDevice::GAME_LEFT])
			{
				current = current->prev;
				_owner->GetComponent<BodyComponent>()->linearStop();
				
				linearMovement = false;
				wallSoundPlayed = false;				

				pressControl[InputDevice::GAME_LEFT] = false;
			}
		}
		else pressControl[InputDevice::GAME_LEFT] = true;

		_owner->GetComponent<BodyComponent>()->setAngle(current->direction);
		//*****************************************************
	}
	else _owner->GetComponent<BodyComponent>()->linearStop();//if backpack is open!
	
	//*********************Backpack**********************
	if(devices -> getInputDevice() -> GetEvent(InputDevice::GAME_B))
	{
		if(pressControl[InputDevice::GAME_B])
		{
			if(BackpackComponent* backpack = _owner->GetComponent<BackpackComponent>();	backpack != nullptr)
			{
				backpack->SetOpen(!backpack->GetOpen());
			}
			pressControl[InputDevice::GAME_B] = false;
		}
		
	}
	else pressControl[InputDevice::GAME_B] = true;
	//****************************************************

	
	//TODO: Notice displays need to be elsewhere. Event handler
	//*****************************NOTICES*********************************************
	//this is the game square in the 15x15 map.
	GAME_VEC square = _owner->GetComponent<BodyComponent>()->getCurrentSquare();
	
	//Get N,S,E,W direction.
	GAME_DIRECTION direction = static_cast<GAME_DIRECTION>(abs((int(devices -> getPhysicsDevice() -> GetAngle(_owner))%360)));
	//set up ntoice
	AssetLibrary::GAME_NOTICE notice = {square, direction, ""};


	//if there is a notice
	notice = devices -> getAssetLibrary() -> getNotice(notice);
	if( notice.text != "")
	{
		//display it.
		devices -> getGraphicsDevice() -> Notice2Screen(notice.text);
	}


	//*********************************************************************************
	
	//TODO: Move level switch to event handler!!! 
	//*****************************TO Basement*********************************************
	//if we are on the main level and make it to the proper spot
	if(devices -> getLevel() == GAME_LEVEL_MAIN && square.x == 14 && square.y == 0)
	{
		//load the basement.
		//NOTE: actual loading of basement is done by the Game class, this just tells the class
		//it is time to load.
		devices -> setLoadBasement(true);
	}
	//*********************************************************************************


	return nullptr;
}

void UserInputComponent::Finish(){}