#include "ComponentsList.h"
#include "DeviceList.h"

#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "NoticesAssetLibrary.h"



UserInputComponent::UserInputComponent(GameObject* owner):Component(owner){}
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

	//use owner's position to scroll when boder approached!
	devices->GetGraphicsDevice()->GetView()->addScroller(_owner);
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
		GAME_INT soundWait = devices->GetFPS() / 5;//adjusts how long between playing of sound effects.
		const std::string walkSound = "walking";
		const std::string runSound = "run";
		std::string sound = walkSound;

		const GAME_INT baseForceMultiplier = 1500; //How fast does the player move.
		GAME_INT forceMultiplier = baseForceMultiplier;
		const GAME_INT runMultiplier = 3; //How many times faster is running then walking

		//*****************Adjustmnts for running************************
		if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_SHIFT))
		{
			forceMultiplier = baseForceMultiplier * runMultiplier;
			soundWait = devices->GetFPS() / 8;
			sound = runSound;
		}
		else
		//back to walking
		{
			soundWait = devices->GetFPS() / 5;
			forceMultiplier = baseForceMultiplier;
			sound = walkSound;
		}
		//*****************************************************************
		//TODO: stop walking sound when button let go of!
		//*******************sound check************
		if (frameCount > soundWait + 1)//walk sound every so often
		{
			devices->GetSoundDevice()->PlaySound(sound, 0, 1);
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
			devices->GetSoundDevice()->PlaySound("wall", 0, 2);
			wallHit = false;
			wallSoundPlayed = true;
		}
		//*****************************************

		//**************Up or Down or neither************
		int forceDirection = 0;
		

		if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_UP))
		{
			forceDirection = -1;
			linearMovement = true;
		}

		else if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_DOWN))
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
			devices->GetPhysicsDevice()->SetLinearVelocity
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
		
		
		
		//TODO: If angles were a linked list instead of an enum, could go forwad and back on it!
		//****************left or right*************
		if(devices -> GetInputDevice() -> GetEvent(InputDevice::GAME_RIGHT))
		{
			if(pressControl[InputDevice::GAME_RIGHT])
			{
				_owner->GetComponent<BodyComponent>()->adjustAngle(90);
				_owner->GetComponent<BodyComponent>()->linearStop();
				pressControl[InputDevice::GAME_RIGHT] = false;
				linearMovement = false;
				wallSoundPlayed = false;
			}
		}
		else pressControl[InputDevice::GAME_RIGHT] = true;

		if(devices -> GetInputDevice() -> GetEvent(InputDevice::GAME_LEFT))
		{
			if(pressControl[InputDevice::GAME_LEFT])
			{
				_owner->GetComponent<BodyComponent>()->adjustAngle(-90);
				_owner->GetComponent<BodyComponent>()->linearStop();
				pressControl[InputDevice::GAME_LEFT] = false;
				linearMovement = false;
				wallSoundPlayed = false;				
			}
		}
		else pressControl[InputDevice::GAME_LEFT] = true;

		//*****************************************************
	}
	else _owner->GetComponent<BodyComponent>()->linearStop();
	
	//*********************Backpack**********************
	if(devices -> GetInputDevice() -> GetEvent(InputDevice::GAME_B))
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

	//HACK: Need to just set angle previously instead of truing up later!
	//*************************************** True to 90**********************
	//sometimes the angle get's off perpendicular.
	int angle = (int)(_owner->GetComponent<BodyComponent>()->getAngle());
	
	if(angle >360 && angle < 460) angle = 90;
	if (angle <0) angle = 270;

	if (angle < 45 && angle >= 315) _owner->GetComponent<BodyComponent>()-> setAngle(N);
	else if (angle < 225 && angle >= 135) _owner->GetComponent<BodyComponent>()->setAngle(S);
	else if (angle < 135 && angle >= 45) _owner->GetComponent<BodyComponent>()->setAngle(E);
	else if (angle < 315 && angle >= 225) _owner->GetComponent<BodyComponent>()->setAngle(W);
	
	//*************************************** BORDER DETECTION**********************

	
	//TODO: Notice displays need to be elsewhere. View???
	//*****************************NOTICES*********************************************
	//this is the game square in the 15x15 map.
	GAME_VEC square = _owner->GetComponent<BodyComponent>()->getCurrentSquare();
	
	//Get N,S,E,W direction.
	GAME_DIRECTION direction = static_cast<GAME_DIRECTION>(abs((int(devices -> GetPhysicsDevice() -> GetAngle(_owner))%360)));
	//set up ntoice
	NoticesAssetLibrary::GAME_NOTICE notice = {square, direction, ""};


	//if there is a notice
	notice = devices -> GetNoticesLibrary() -> Search(notice);
	if( notice.text != "")
	{
		//display it.
		devices -> GetGraphicsDevice() -> Notice2Screen(notice.text);
	}


	//*********************************************************************************
	
	//TODO: Move level switch to event handler!!! 
	//*****************************TO Basement*********************************************
	//if we are on the main level and make it to the proper spot
	if(devices -> GetLevel() == GAME_LEVEL_MAIN && square.x == 14 && square.y == 0)
	{
		//load the basement.
		//NOTE: actual loading of basement is done by the Game class, this just tells the class
		//it is time to load.
		devices -> SetLoadBasement(true);
	}
	//*********************************************************************************


	return nullptr;
}

void UserInputComponent::Finish(){}