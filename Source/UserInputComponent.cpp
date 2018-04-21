#include "ComponentsList.h"
#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"


UserInputComponent::UserInputComponent(std::shared_ptr<GameObject> owner):Component(owner){}
UserInputComponent::~UserInputComponent(){}

//**************************************
//Set up some defaults
bool UserInputComponent::Initialize(GAME_OBJECTFACTORY_PRESETS& presets)
{
	frameCount = 0;
	zeroVec.x = 0;
	zeroVec.y = 0;
	devices = presets.devices;
	wallHit = false;
	noWall =true;

	for (int i = 0; i < GAME_NUM_EVENTS; i++)
	{
		GAME_EVENT eventNum = static_cast<GAME_EVENT>(i);
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
	frameCount++;
	bool locationMarker = false;
	GAME_VEC applyForce;
	GAME_INT baseForceMultiplier = 1500; //How fast does the player move.
	GAME_INT forceMultiplier = baseForceMultiplier;
	GAME_INT runMultiplier = 3; //How many times faster is running then walking
	GAME_INT angleAdjust = 90; //How many degrees does the player turn each time
	GAME_INT soundWait = devices -> GetFPS()/5;//adjusts how long between playing of sound effects.
	
	std::string sound = "walking";

	if(!_owner -> GetComponent<BackpackComponent>() -> GetOpen())
	{
		//Run if shift is being held down.
		if(devices -> GetInputDevice() -> GetEvent(GAME_SHIFT))
		{
			forceMultiplier = baseForceMultiplier * runMultiplier;
			//play it faster
			soundWait = devices -> GetFPS()/8;
			//do the run sound
			sound = "run";
		}
		else
		{
			//back to walk speeds
			forceMultiplier = baseForceMultiplier;
		
		}
	
		//checks to see if it is time to play the sound effect or not.
		bool playWalk = false;
		if(frameCount > soundWait+1) 
		{
			frameCount = soundWait +1;
			playWalk = true;
		}

		//grab current velocity
		GAME_VEC velocity = devices -> GetPhysicsDevice() -> GetVelocity(_owner.get());
			

		//If the up or down arrow is pressed
		if(devices -> GetInputDevice() -> GetEvent(GAME_UP))
		{
			//Calculate force vector for a forward push
			applyForce.x = (float)cosf((devices -> GetPhysicsDevice() -> GetAngle(_owner.get())*PI/180)-(PI/2))*forceMultiplier;
			applyForce.y = (float)sinf((devices -> GetPhysicsDevice() -> GetAngle(_owner.get())*PI/180)-(PI/2))*forceMultiplier;
			//PUSH BABY PUSH!!!
			devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner.get(), applyForce);
			//if last play is done and the player is still moving
			if(playWalk ==true && (abs(velocity.x) > 1 || abs(velocity.y) >1))
			{
				//play walking sound
				devices -> GetSoundDevice() -> PlaySound(sound,0,1);
				frameCount = 0;
			}
		
			//nowall makes sure we don't play the wall hitting sound if we are parallel with and next to
			//a wall. Without out it, the wall hit sound will play once we when we start moving.
			//wallHit is set by the contactListener
			//If we are hitting a wall we should not be moving, so velocity is checked.
			//there usually is some very small velocity even when running into a wall, which is why it checks
			//for less than one.
			if(!noWall && wallHit && abs(velocity.x) <1 && abs(velocity.y) < 1)
			{
				devices -> GetSoundDevice() -> PlaySound("wall",0,2);
			}
			noWall = false;

		}
		//otherwise, if the down arrow is pressed.
		else if(devices -> GetInputDevice() -> GetEvent(GAME_DOWN))
		{
			//Force reversed from the UP force by adding PI to the angle.
			applyForce.x = (float)cosf((devices -> GetPhysicsDevice() -> GetAngle(_owner.get())*PI/180)+(PI/2))*forceMultiplier;
			applyForce.y = (float)sinf((devices -> GetPhysicsDevice() -> GetAngle(_owner.get())*PI/180)+(PI/2))*forceMultiplier;

			//PUSH BABY PUSH!!!
			devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner.get(), applyForce);
			//if last play is done and still moving
			if(playWalk ==true && (abs(velocity.x) > 1 || abs(velocity.y) >1))
			{
				//play walking sound
				devices -> GetSoundDevice() -> PlaySound(sound,0,1);
			
				frameCount = 0;
			}
			//same as up arrow....
			if(!noWall && wallHit && abs(velocity.x) <1 && abs(velocity.y) < 1)
			{
				devices -> GetSoundDevice() -> PlaySound("wall",0,2);
			}
			noWall = false;

		
		}
		//if neither are pressed, remove velocity.
		else
		{
			devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner.get(), zeroVec);
			//make sure we won't make a wall sound the first frame after we press an up or down arrow.
			noWall = true;
		}
	
		//Check for left or right buttons
		// the "turn" variable makes sure we only turn once every time we push the button
		if(devices -> GetInputDevice() -> GetEvent(GAME_RIGHT))
		{
			//change the angle
			if(pressControl[GAME_RIGHT]) 
			{
				//set angle by angle adjust
				devices -> GetPhysicsDevice() -> SetAngle(
					_owner.get(), 
					devices -> GetPhysicsDevice() -> GetAngle(_owner.get())+angleAdjust);
				//stop forward motion
				devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner.get(), zeroVec);
				//ensures we only turn once per press
				pressControl[GAME_RIGHT] = false;
			}
			//same as else on up and down arrows
			noWall = true;
		}
		else pressControl[GAME_RIGHT] = true;
		if(devices -> GetInputDevice() -> GetEvent(GAME_LEFT))
		{
			//similar to turn right.
			if(pressControl[GAME_LEFT]) 
			{
				devices -> GetPhysicsDevice() -> SetAngle(_owner.get(), devices -> GetPhysicsDevice() -> GetAngle(_owner.get())-angleAdjust);
				devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner.get(), zeroVec);
				pressControl[GAME_LEFT] = false;
			}
			//same as else on up and down arrows
			noWall = true;
		
		}
		else pressControl[GAME_LEFT] = true;
	}
	else devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner.get(), zeroVec);
	
	if(devices -> GetInputDevice() -> GetEvent(GAME_B))
	{
		if(pressControl[GAME_B])
		{
			std::shared_ptr<BackpackComponent> backpack = _owner -> GetComponent<BackpackComponent>();	
			if(backpack != NULL)
			{
				if(backpack -> GetOpen()) backpack -> SetOpen(false);
				else backpack -> SetOpen(true);
			}
			pressControl[GAME_B] = false;
		}
		
	}
	else pressControl[GAME_B] = true;



	//*************************************** True to 90**********************
	//sometimes the angle get's off perpendicular.
	int angle = (int(devices -> GetPhysicsDevice() -> GetAngle(_owner.get())));
	
	if(angle >360 && angle < 460) angle = 90;
	if (angle <0) angle = 270;
	if (angle < 45 && angle >= 315) devices -> GetPhysicsDevice() -> SetAngle(_owner.get(), 0);
	else if (angle < 135 && angle >= 45) devices -> GetPhysicsDevice() -> SetAngle(_owner.get(), 90);
	else if (angle < 225 && angle >= 135) devices -> GetPhysicsDevice() -> SetAngle(_owner.get(), 180);
	else if (angle < 315 && angle >= 225) devices -> GetPhysicsDevice() -> SetAngle(_owner.get(), 270);



	//*************************************** BORDER DETECTION**********************
		
	//Disntance from edge of screen before screen updates.	
	GAME_INT border = 200;
	
	//the amount the screen moves is based upon the last change in position for the player.
	GAME_VEC jump = _owner -> GetComponent<BodyComponent>() -> GetDPosition();
	
	//grab a few things we are going to need.
	PhysicsDevice* pDevice = devices -> GetPhysicsDevice();
	GraphicsDevice* gDevice = devices -> GetGraphicsDevice();
	std::shared_ptr<View> view = devices -> GetGraphicsDevice() -> GetView();
	
	//position plus view
	GAME_VEC position = _owner -> GetComponent<RendererComponent>() -> GetUpdatedPosition(_owner);
	
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
	GAME_NOTICE notice = {square.x, square.y, direction, ""};


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

	if(locationMarker)
	{
		//GAME_VEC playerPosition = devices -> GetPhysicsDevice() -> GetLinearVelocity(_owner.get());
		GAME_VEC playerPosition = devices -> GetPhysicsDevice() -> GetPosition(_owner.get());
		std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
		devices -> GetGraphicsDevice() ->Text2Screen(playerPositionText, position);
	}

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
						 _owner -> GetComponent<RendererComponent>() -> GetUpdatedPosition(_owner).x,
						 _owner -> GetComponent<RendererComponent>() -> GetUpdatedPosition(_owner).y
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
	
	GAME_VEC position = _owner -> GetComponent<RendererComponent>() -> GetUpdatedPosition(devices -> GetPhysicsDevice() -> GetPosition(_owner.get()));
	devices -> GetGraphicsDevice() ->Text2Screen(playerPositionText, position);*/

}
void UserInputComponent::Finish(){}