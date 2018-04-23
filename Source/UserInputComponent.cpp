#include "ComponentsList.h"
#include "DeviceList.h"

#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "NoticesAssetLibrary.h"



UserInputComponent::UserInputComponent(std::shared_ptr<GameObject> owner):Component(owner){}
UserInputComponent::~UserInputComponent(){}

//**************************************
//Set up some defaults
bool UserInputComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
{
	
	devices = presets.devices;
	wallHit = false;
	noWallSound =true;

	for (int i = 0; i < InputDevice::GAME_NUM_EVENTS; i++)
	{
		InputDevice::GAME_EVENT eventNum = static_cast<InputDevice::GAME_EVENT>(i);
		pressControl[eventNum] = true;
	}


	return true;
}
	
void UserInputComponent::Start(){}

//**************************************
//reacts to keyboard input and adjusts the world accoringly.
std::shared_ptr<GameObject> UserInputComponent::Update()
//**************************************
{
	static int frameCount = 0;
	
	if(!_owner -> GetComponent<BackpackComponent>() -> GetOpen())
	{
		GAME_INT soundWait = devices->GetFPS() / 5;//adjusts how long between playing of sound effects.
		std::string walkSound = "walking";
		std::string runSound = "run";
		std::string sound = walkSound;

		GAME_INT baseForceMultiplier = 1500; //How fast does the player move.
		GAME_INT forceMultiplier = baseForceMultiplier;
		GAME_INT runMultiplier = 3; //How many times faster is running then walking

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
	
		//*******************sound check************
		if (frameCount > soundWait + 1)//walk sound every so often
		{
			devices->GetSoundDevice()->PlaySound(sound, 0, 1);
			frameCount = 0;
		}
		//wal sound based on forward/backward motion and collision detection
		if (!noWallSound && wallHit && abs(_owner->GetComponent<BodyComponent>()->getVelocity()) < 1)
		{
			devices->GetSoundDevice()->PlaySound("wall", 0, 2);
			wallHit = false;
		}
		//*****************************************

		//**************Up or Down or neither************
		int forceDirection = 1;


		if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_UP))
		{
			forceDirection = -1;
		}
		else if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_DOWN))
		{
			forceDirection = 1;
		}
		else
		{
			forceDirection = 0;
		}
		//***********************************************

		//****************MOVE!*****************
		if (forceDirection != 0)
		{
			//PUSH BABY PUSH!!!
			devices->GetPhysicsDevice()->SetLinearVelocity
			(
				_owner.get(),
				//x=(cos(angle) + or - (PI/2)) * (force)
				//y=(sin(angle) + or - (PI/2)) * (force)
				{ 
					(float)cosf((_owner->GetComponent<BodyComponent>()->GetAngle()*PI / 180) + forceDirection * (PI / 2))*forceMultiplier,
					(float)sinf((_owner->GetComponent<BodyComponent>()->GetAngle()*PI / 180) + forceDirection * (PI / 2))*forceMultiplier
				}
			);
			//can play wall sound upon collision!
			noWallSound = false;
			frameCount++;
		}
		//Don't move!
		else
		{
			_owner->GetComponent<BodyComponent>()->linearStop();
			noWallSound = true;
			frameCount = 0;
			
		}
		//****************************************
		
		
		
	
		//****************left or right*************
		if(devices -> GetInputDevice() -> GetEvent(InputDevice::GAME_RIGHT))
		{
			if(pressControl[InputDevice::GAME_RIGHT])
			{
				_owner->GetComponent<BodyComponent>()->adjustAngle(90);
				_owner->GetComponent<BodyComponent>()->linearStop();
				pressControl[InputDevice::GAME_RIGHT] = false;
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
			if(std::shared_ptr<BackpackComponent> backpack = _owner->GetComponent<BackpackComponent>();	backpack != nullptr)
			{
				backpack->SetOpen(!backpack->GetOpen());
			}
			pressControl[InputDevice::GAME_B] = false;
		}
		
	}
	else pressControl[InputDevice::GAME_B] = true;
	//****************************************************


	//*************************************** True to 90**********************
	//sometimes the angle get's off perpendicular.
	int angle = (int)(_owner->GetComponent<BodyComponent>()->GetAngle());
	
	if(angle >360 && angle < 460) angle = 90;
	if (angle <0) angle = 270;

	if (angle < 45 && angle >= 315) _owner->GetComponent<BodyComponent>()-> setAngle(N);
	else if (angle < 225 && angle >= 135) _owner->GetComponent<BodyComponent>()->setAngle(S);
	else if (angle < 135 && angle >= 45) _owner->GetComponent<BodyComponent>()->setAngle(E);
	else if (angle < 315 && angle >= 225) _owner->GetComponent<BodyComponent>()->setAngle(W);
	
	//*************************************** BORDER DETECTION**********************

	GraphicsDevice* gDevice = devices->GetGraphicsDevice();
	View* view = devices->GetGraphicsDevice()->GetView();
	const GAME_INT border = 200;
	const GAME_VEC position = _owner -> GetComponent<RendererComponent>() -> GetViewAdjustedPosition(_owner);
	
	//the amount the screen moves is based upon the last change in position for the player.
	static GAME_VEC oldPosition = _owner->GetComponent<BodyComponent>()->getPosition();
	GAME_VEC jump = _owner->GetComponent<BodyComponent>()->getPosition() - oldPosition;
	oldPosition = _owner->GetComponent<BodyComponent>()->getPosition();

	//left
	if(position.x < border)
	{
		view -> setX(view -> getPosition().x-jump.x);				
	}
	//right
	else if(position.x > gDevice -> GetScreenWidth() - border)
	{ 
		view -> setX(view -> getPosition().x-jump.x);
	}
	//top
	if(position.y < border)
	{
		view -> setY(view -> getPosition().y-jump.y);				
	}
	//bottom
	else if(position.y  > gDevice -> GetScreenHeight() - border)
	{
		view -> setY(view -> getPosition().y-jump.y);
	}
		
	//*********************************************************************************

	//*****************************NOTICES*********************************************
	//this is the game square in the 15x15 map.
	GAME_VEC square = GetCurrentSquare();
	
	//Get N,S,E,W direction.
	GAME_DIRECTION direction = static_cast<GAME_DIRECTION>(abs((int(devices -> GetPhysicsDevice() -> GetAngle(_owner.get()))%360)));
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
	
	//Move to somewhere else!!! FIXME
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


	return NULL;
}

//**************************************
//find's the 15x15 game square based on current position
GAME_VEC UserInputComponent::GetCurrentSquare()
//**************************************
{
	//view adjusted player position.
	GAME_VEC playerPosition = 
					{
						 _owner -> GetComponent<RendererComponent>() -> GetViewAdjustedPosition(_owner).x,
						 _owner -> GetComponent<RendererComponent>() -> GetViewAdjustedPosition(_owner).y
					};
	//view vector.
	GAME_VEC viewPosition = 
					{
						devices -> GetGraphicsDevice() -> GetView() -> getPosition().x,
						devices -> GetGraphicsDevice() -> GetView() -> getPosition().y
					};
	//The coordinates of the original top left city corner.
	GAME_VEC cityCorner = 
					{
						devices -> GetCityCorner().x,
						devices -> GetCityCorner().y
					};
	//the city corner plus the view get's us the top left corner of the view.
	//subtract off the player's position on the screen to get the actual spot of the player.
	//divide by 110 which is the number of pixels in each square.
	//Adjust the y, because the 15x15 square starts in the bottom left corner, while SDL starts in the top left.
	GAME_VEC square = 
					{
						int((cityCorner.x + viewPosition.x - playerPosition.x)*-1/110),
						15+int((cityCorner.y + viewPosition.y - playerPosition.y)/110)
					};
	return square;					
	//this code is here for use for debugging.
	//Draw player position on himself
	/*std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
	
	GAME_VEC position = _owner -> GetComponent<RendererComponent>() -> GetViewAdjustedPosition(devices -> GetPhysicsDevice() -> GetPosition(_owner.get()));
	devices -> GetGraphicsDevice() ->Text2Screen(playerPositionText, position);*/

}
void UserInputComponent::Finish(){}