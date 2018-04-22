#include "ComponentsList.h"
#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "InputDevice.h"
#include "SoundDevice.h"
#include "View.h"
#include "GraphicsDevice.h"
#include "NoticesAssetLibrary.h"
#include "Blackboard.h"
#include "PhysicsDevice.h"


UserInputComponent::UserInputComponent(std::shared_ptr<GameObject> owner):Component(owner){}
UserInputComponent::~UserInputComponent(){}

//**************************************
//Set up some defaults
bool UserInputComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets) 
{
	devices = presets.devices;
	wallHit = false;
	noWallSound = true;
	
	for (int i = 0; i < InputDevice::GAME_NUM_EVENTS; i++)
	{
		pressControl[(InputDevice::GAME_EVENT)i] = true;
	}
	if (devices!=nullptr)
		return true;
	return false;
}
	
void UserInputComponent::Start(){}

//**************************************
//reacts to keyboard input and adjusts the world accoringly.
std::shared_ptr<GameObject> UserInputComponent::Update()
//**************************************
{
	
	//for sound play
	static GAME_INT frameCount = 0;
	frameCount++;
	
	//bool locationMarker = false;	

	if (!_owner->GetComponent<BackpackComponent>()->GetOpen())
	{

		GAME_INT soundWait = devices->GetFPS() / 5;//adjusts how long between playing of sound effects.
		std::string walkSound = "walking";
		std::string runSound = "run";
		std::string sound = walkSound;


		const GAME_INT baseForceMultiplier = 1500; //How fast does the player move.
		GAME_INT forceMultiplier = baseForceMultiplier;
		const GAME_INT runMultiplier = 3; //How many times faster is running then walking

		/*****************Adjustmnts for running************************/
		if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_SHIFT))
		{
			forceMultiplier = baseForceMultiplier * runMultiplier;
			//play it faster
			soundWait = devices->GetFPS() / 8;
			//do the run sound
			sound = runSound;
		}
		else
		{
			//back to walk speeds
			forceMultiplier = baseForceMultiplier;
			sound = walkSound;
		}
		/*****************************************************************/

		/*******************sound check************/
		if (frameCount > soundWait + 1)//walk sound every so often
		{
			devices->GetSoundDevice()->PlaySound(sound, 0, 1);
			frameCount = 0;
		}
		//wal sound based on forward/backward motion and collision detection
		if (!noWallSound && wallHit)
		{
			devices->GetSoundDevice()->PlaySound("wall", 0, 2);
		}
		/*****************************************/

		/**************Up or Down or neither************/
		int forceDirection = 1;
		

		if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_UP))
		{	forceDirection = 1;}
		else if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_DOWN))
		{	forceDirection = -1;}
		else
		{
			forceDirection = 0;
		}
		/***********************************************/
		
		/****************MOVE!*****************/
		if (forceDirection != 0)
		{
			//PUSH BABY PUSH!!!
			devices->GetPhysicsDevice()->SetLinearVelocity
			(
				_owner.get(),
				//x=(cos(angle) + or - (PI/2)) * (force)
				//y=(sin(angle) + or - (PI/2)) * (force)
				{(float)cosf((devices->GetPhysicsDevice()->GetAngle(_owner.get())*PI / 180) + forceDirection * (PI / 2))*forceMultiplier,
				(float)sinf((devices->GetPhysicsDevice()->GetAngle(_owner.get())*PI / 180) + forceDirection * (PI / 2))*forceMultiplier }
			);
			//can play wall sound upon collision!
			noWallSound = false;
		}
		//Don't move!
		else
		{
			_owner->GetComponent<BodyComponent>()->linearStop();
			noWallSound = true;
		}
		/****************************************/

		/****************left or right*************/
		GAME_INT angleAdjust = 90; //How many degrees does the player turn each time
		if(devices -> GetInputDevice() -> GetEvent(InputDevice::GAME_RIGHT) || devices->GetInputDevice()->GetEvent(InputDevice::GAME_LEFT))
		{
			if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_RIGHT))
			{
				angleAdjust = 90;
				pressControl[InputDevice::GAME_RIGHT] = false;
			}
			else
				pressControl[InputDevice::GAME_RIGHT] = true;

			if (devices->GetInputDevice()->GetEvent(InputDevice::GAME_LEFT))
			{
				angleAdjust = -90;
				pressControl[InputDevice::GAME_LEFT] = false;
			}
			else
				pressControl[InputDevice::GAME_LEFT] = true;

			//set angle by angle adjust
			_owner->GetComponent<BodyComponent>()->adjustAngle(angleAdjust);
			//stop forward motion
			_owner->GetComponent<BodyComponent>()->linearStop();
			//ensures we only turn once per press
		}
		/*******************************************/
	}
	else _owner->GetComponent<BodyComponent>()->linearStop();
	
	//****************Backpack**********************
	if(devices -> GetInputDevice() -> GetEvent(InputDevice::GAME_B))
	{
		if(pressControl[InputDevice::GAME_B])
		{
			BackpackComponent* backpack = _owner -> GetComponent<BackpackComponent>().get();	
			if(backpack != nullptr)
			{
				backpack->SetOpen(!backpack->GetOpen());
			}
			pressControl[InputDevice::GAME_B] = false;
		}
		
	}
	else pressControl[InputDevice::GAME_B] = true;
	//************************************************


	//*************************************** True to 90**********************
	//sometimes the angle get's off perpendicular.
	int angle = (int(_owner->GetComponent<BodyComponent>()-> GetAngle()));
	
	if(angle >360 && angle < 460) angle = 90;
	if (angle <0) angle = 270;

	if (angle < 45 && angle >= 315) _owner->GetComponent<BodyComponent>()->setAngle(N);
	else if (angle < 225 && angle >= 135) _owner->GetComponent<BodyComponent>()->setAngle(S);
	else if (angle < 135 && angle >= 45) _owner->GetComponent<BodyComponent>()->setAngle(E);
	else if (angle < 315 && angle >= 225) _owner->GetComponent<BodyComponent>()->setAngle(W);
	//***************************************************************************


	//*************************************** BORDER DETECTION**********************
	//Disntance from edge of screen before screen updates.	
	static const GAME_INT border = 200;
		
	//grab a few things we are going to need.
	PhysicsDevice* pDevice = devices -> GetPhysicsDevice();
	GraphicsDevice* gDevice = devices -> GetGraphicsDevice();
	View* view = devices -> GetGraphicsDevice() -> GetView();
	
	//the amount the screen moves is based upon the last change in position for the player.
	devices->getBlackboard()->updateJump(_owner->GetComponent<BodyComponent>()->getPosition());
	GAME_VEC jump = devices->getBlackboard()->getJump();
	
	//position plus view
	GAME_VEC position = _owner -> GetComponent<RendererComponent>() -> GetViewAdjustedPosition();
	
	//Check distance for each border and adjust view if too close.
	//it is alway "-" jump because the DPosition's sign changes based on the direction of movement
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
	NoticesAssetLibrary::GAME_NOTICE notice = {(GAME_INT)square.x, (GAME_INT)square.y, direction, ""};


	//if there is a notice
	notice = devices -> GetNoticesLibrary() -> Search(notice);
	if( notice.text != "")
	{
		//display it.
		devices -> GetGraphicsDevice() -> Notice2Screen(notice.text);
	}


	//*********************************************************************************
	
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

	//if(locationMarker)
	//{
	//	//GAME_VEC playerPosition = devices -> GetPhysicsDevice() -> GetLinearVelocity(_owner.get());
	//	GAME_VEC playerPosition = _owner->GetComponent<BodyComponent>()->getPosition();
	//	std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
	//	devices -> GetGraphicsDevice() ->Text2Screen(playerPositionText, position);
	//}

	return NULL;
}

//**************************************
//find's the 15x15 game square based on current position
GAME_VEC UserInputComponent::GetCurrentSquare()
//**************************************
{
	//view adjusted player position.
	GAME_VEC playerPosition = _owner->GetComponent<RendererComponent>()->GetViewAdjustedPosition();

	//view vector.
	GAME_VEC viewPosition = _owner->GetComponent<RendererComponent>()->GetViewAdjustedPosition();
					
	//The coordinates of the original top left city corner.
	GAME_VEC cityCorner = devices->GetCityCorner();
					
	//the city corner plus the view get's us the top left corner of the view.
	//subtract off the player's position on the screen to get the actual spot of the player.
	//divide by 110 which is the number of pixels in each square.
	//Adjust the y, because the 15x15 square starts in the bottom left corner, while SDL starts in the top left.
	GAME_VEC square = 
					{
						(GAME_FLT)((cityCorner.x + viewPosition.x - playerPosition.x)*-1/110),
						(GAME_FLT)(15+int((cityCorner.y + viewPosition.y - playerPosition.y)/110))
					};
						
	//this code is here for use for debugging.
	//Draw player position on himself
	std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
	
	//GAME_VEC position = _owner -> GetComponent<RendererComponent>() -> GetViewAdjustedPosition();
	//devices -> GetGraphicsDevice() ->Text2Screen(playerPositionText, position);
	devices->GetGraphicsDevice()->Text2Screen(playerPositionText, { 0, 0});
	return square;
}
void UserInputComponent::Finish(){}