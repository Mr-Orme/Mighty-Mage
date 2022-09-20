#include "ComponentsList.h"
#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "GameObject.h"


UserInputComponent::UserInputComponent(std::unique_ptr<GameObject> owner):Component(owner){}
UserInputComponent::~UserInputComponent(){}

//**************************************
//Set up some defaults
bool UserInputComponent::initialize(ObjectFactoryPresets& presets)
{
	frameCount = 0;
	zeroVec.x = 0;
	zeroVec.y = 0;
	devices = presets.devices;
	wallHit = false;
	noWall =true;

	for (int i = 0; i < (int)Event::numEvents; i++)
	{
		Event eventNum = static_cast<Event>(i);
		pressControl[eventNum] = true;
	}


	return true;
}
	
void UserInputComponent::Start(){}

//**************************************
//reacts to keyboard input and adjusts the world accoringly.
std::unique_ptr<GameObject> UserInputComponent::update()
//**************************************
{
	frameCount++;
	bool locationMarker = false;
	Vector2D applyForce;
	int baseForceMultiplier = 1500; //How fast does the player move.
	int forceMultiplier = baseForceMultiplier;
	int runMultiplier = 3; //How many times faster is running then walking
	int angleAdjust = 90; //How many degrees does the player turn each time
	int soundWait = devices -> GetFPS()/5;//adjusts how long between playing of sound effects.
	
	std::string sound = "walking";

	if(!_owner -> getComponent<BackpackComponent>() -> GetOpen())
	{
		//run if shift is being held down.
		if(devices -> GetInputDevice() -> GetEvent(Event::shift))
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
		Vector2D velocity = devices -> GetPhysicsDevice() -> GetVelocity(_owner);
			

		//If the up or down arrow is pressed
		if(devices -> GetInputDevice() -> GetEvent(Event::up))
		{
			//Calculate force vector for a forward push
			applyForce.x = (float)cosf((devices -> GetPhysicsDevice() -> getAngle(_owner)*PI/180)-(PI/2))*forceMultiplier;
			applyForce.y = (float)sinf((devices -> GetPhysicsDevice() -> getAngle(_owner)*PI/180)-(PI/2))*forceMultiplier;
			//PUSH BABY PUSH!!!
			devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner, applyForce);
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
		else if(devices -> GetInputDevice() -> GetEvent(Event::down))
		{
			//Force reversed from the UP force by adding PI to the angle.
			applyForce.x = (float)cosf((devices -> GetPhysicsDevice() -> getAngle(_owner)*PI/180)+(PI/2))*forceMultiplier;
			applyForce.y = (float)sinf((devices -> GetPhysicsDevice() -> getAngle(_owner)*PI/180)+(PI/2))*forceMultiplier;

			//PUSH BABY PUSH!!!
			devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner, applyForce);
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
			devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner, zeroVec);
			//make sure we won't make a wall sound the first frame after we press an up or down arrow.
			noWall = true;
		}
	
		//Check for left or right buttons
		// the "turn" variable makes sure we only turn once every time we push the button
		if(devices -> GetInputDevice() -> GetEvent(Event::right))
		{
			//change the angle
			if(pressControl[Event::right])
			{
				//set angle by angle adjust
				devices -> GetPhysicsDevice() -> SetAngle(
					_owner, 
					devices -> GetPhysicsDevice() -> getAngle(_owner)+angleAdjust);
				//stop forward motion
				devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner, zeroVec);
				//ensures we only turn once per press
				pressControl[Event::right] = false;
			}
			//same as else on up and down arrows
			noWall = true;
		}
		else pressControl[Event::right] = true;
		if(devices -> GetInputDevice() -> GetEvent(Event::left))
		{
			//similar to turn right.
			if(pressControl[Event::left])
			{
				devices -> GetPhysicsDevice() -> SetAngle(_owner, devices -> GetPhysicsDevice() -> getAngle(_owner)-angleAdjust);
				devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner, zeroVec);
				pressControl[Event::left] = false;
			}
			//same as else on up and down arrows
			noWall = true;
		
		}
		else pressControl[Event::left] = true;
	}
	else devices -> GetPhysicsDevice() -> SetLinearVelocity(_owner, zeroVec);
	
	if(devices -> GetInputDevice() -> GetEvent(Event::key_b))
	{
		if(pressControl[Event::key_b])
		{
			std::unique_ptr<BackpackComponent> backpack = _owner -> getComponent<BackpackComponent>();	
			if(backpack != nullptr)
			{
				if(backpack -> GetOpen()) backpack -> SetOpen(false);
				else backpack -> SetOpen(true);
			}
			pressControl[Event::key_b] = false;
		}
		
	}
	else pressControl[Event::key_b] = true;



	//*************************************** True to 90**********************
	//sometimes the angle get's off perpendicular.
	int angle = (int(devices -> GetPhysicsDevice() -> getAngle(_owner)));
	
	if(angle >360 && angle < 460) angle = 90;
	if (angle <0) angle = 270;
	if (angle < 45 && angle >= 315) devices -> GetPhysicsDevice() -> SetAngle(_owner, 0);
	else if (angle < 135 && angle >= 45) devices -> GetPhysicsDevice() -> SetAngle(_owner, 90);
	else if (angle < 225 && angle >= 135) devices -> GetPhysicsDevice() -> SetAngle(_owner, 180);
	else if (angle < 315 && angle >= 225) devices -> GetPhysicsDevice() -> SetAngle(_owner, 270);



	//*************************************** BORDER DETECTION**********************
		
	//Disntance from edge of screen before screen updates.	
	int border = 200;
	
	//the amount the screen moves is based upon the last change in position for the player.
	Vector2D jump = _owner -> getComponent<BodyComponent>() -> getPosition();
	
	//grab a few things we are going to need.
	PhysicsDevice* pDevice = devices -> GetPhysicsDevice();
	GraphicsDevice* gDevice = devices -> GetGraphicsDevice();
	std::unique_ptr<View> view = devices -> GetGraphicsDevice() -> GetView();
	
	//position plus view
	Vector2D position = _owner -> getComponent<RendererComponent>() -> GetUpdatedPosition(_owner);
	
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
	Vector2D square = GetCurrentSquare();
	
	//Get N,S,E,W direction.
	Direction direction = static_cast<Direction>(abs((int(devices -> GetPhysicsDevice() -> getAngle(_owner))%360)));
	//set up ntoice
	Notice notice = {square.x, square.y, direction, ""};


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
	if(devices -> GetLevel() == Levels::main && square.x == 14 && square.y == 0)
	{
		//load the basement.
		//NOTE: actual loading of basement is done by the Game class, this just tells the class
		//it is time to load.
		devices -> SetLoadBasement(true);
	}
	//*********************************************************************************

	if(locationMarker)
	{
		//Vector2D playerPosition = devices -> GetPhysicsDevice() -> GetLinearVelocity(_owner);
		Vector2D playerPosition = devices -> GetPhysicsDevice() -> GetPosition(_owner);
		std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
		devices -> GetGraphicsDevice() ->Text2Screen(playerPositionText, position);
	}

	return nullptr;
}

//**************************************
//find's the 15x15 game square based on current position
Vector2D UserInputComponent::GetCurrentSquare()
//**************************************
{
	//view adjusted player position.
	Vector2D playerPosition = 
					{
						 _owner -> getComponent<RendererComponent>() -> GetUpdatedPosition(_owner).x,
						 _owner -> getComponent<RendererComponent>() -> GetUpdatedPosition(_owner).y
					};
	//view vector.
	Vector2D viewPosition = 
					{
						devices -> GetGraphicsDevice() -> GetView() -> getPosition().x,
						devices -> GetGraphicsDevice() -> GetView() -> getPosition().y
					};
	//The coordinates of the original top left city corner.
	Vector2D cityCorner = 
					{
						devices -> GetCityCorner().x,
						devices -> GetCityCorner().y
					};
	//the city corner plus the view get's us the top left corner of the view.
	//subtract off the player's position on the screen to get the actual spot of the player.
	//divide by 110 which is the number of pixels in each square.
	//Adjust the y, because the 15x15 square starts in the bottom left corner, while SDL starts in the top left.
	Vector2D square = 
					{
						int((cityCorner.x + viewPosition.x - playerPosition.x)*-1/110),
						15+int((cityCorner.y + viewPosition.y - playerPosition.y)/110)
					};
	return square;					
	//this code is here for use for debugging.
	//run player position on himself
	/*std::string playerPositionText = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
	
	Vector2D position = _owner -> getComponent<RendererComponent>() -> GetUpdatedPosition(devices -> GetPhysicsDevice() -> GetPosition(_owner));
	devices -> GetGraphicsDevice() ->Text2Screen(playerPositionText, position);*/

}
void UserInputComponent::Finish(){}