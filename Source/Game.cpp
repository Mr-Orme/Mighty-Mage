#include "Game.h"
#include "ComponentsList.h"
#include "FrameCounter.h"
#include "Timer.h"
#include "Definitions.h"
#include "GameObject.h"
#include "Initializers.h"
#include "tinyxml2.h"
Game::Game()
{}

Game::Game(std::string levelConfigFile, std::string assetConfigFile)
{
	loadLevel(levelConfigFile, assetConfigFile);
}

Game::~Game()
{}


//**************************************
//Takes a string corresponding to an XML file
//containing level config data and loads the objects
//into the objects vector.
//Also creates a new view object for the level.
bool Game::loadLevel(std::string levelConfig, std::string assetConfigFile)
//**************************************
{

	reset();
	//========================================
	//Construct Device Manager
	//========================================
	devices = std::make_unique<ResourceManager>(screenDimensions, assetConfigFile);
	frameTimer = std::make_unique<Timer>(devices->GetFPS());
	///Get a few things ready
	ObjectFactoryPresets presets;
	presets.devices = devices.get();
	ObjectFactory* objectFactory{ devices->GetObjectFactory() };

	//========================================
	//load the files
	//========================================
	tinyxml2::XMLDocument currentLevel;
	//if it does not load, the level or the physics did not load.
	if (!currentLevel.LoadFile(levelConfig.c_str())==tinyxml2::XML_SUCCESS){ return false; };
	//create a root variable and set to the first element "Level"
	tinyxml2::XMLElement* lRoot = currentLevel.FirstChildElement();;
	//record the level;
	int iLevel;
	lRoot->QueryIntAttribute("level", &iLevel);
	devices->SetLevel(static_cast<Levels>(iLevel));



	//get the first child element "Row"
	tinyxml2::XMLElement* rowElement = lRoot->FirstChildElement();


	//Size, in pixels, of one square in the game space
	int squareDimension = 110;
	presets.bodyInitializers.position.x = 0;
	presets.bodyInitializers.position.y = 0;
	//keeps track of the game square we are currently on.
	Vector2D squarePosition = { presets.bodyInitializers.position.x, presets.bodyInitializers.position.y };

	//========================================
	//Add level elements to object array
	//========================================
	do
	{
		//get's the first square;
		tinyxml2::XMLElement* squareElement = rowElement->FirstChildElement();
		//This let's us know if we are doing extras or building actual squares.
		std::string label = rowElement->Value();
		do
		{
			if (label == "Extras") // not walls or doors
			{
				//what type of extra is it?
				presets.objectType = squareElement->Value();

				float temp;
				//grab the x value, this is the "Square" of the map where it is located.
				squareElement->QueryFloatAttribute("x", &temp);
				//translate to pixel positoin for Graphics Device
				presets.bodyInitializers.position.x = (int)((temp *squareDimension) + squarePosition.x);

				//grab the y value
				squareElement->QueryFloatAttribute("y", &temp);

				//separate into fraction and integerparts;
				float fractPart, intPart;
				fractPart = modff(temp, &intPart);
				//The SDL top starts at 0, Our map is reversed, so we need to subtract the integer part from 15 and add back on the fractinal part.
				presets.bodyInitializers.position.y = (int)((((15 - intPart) + fractPart) * squareDimension) + squarePosition.y);

				//grab the angle
				squareElement->QueryFloatAttribute("angle", &presets.bodyInitializers.angle);

				//adjusts where we start the level based on the player's position
				//and puts the player in the middle of the screen.
				//PLAYER MUST BE THE FIRST ITEM IN THE XML IN ORDER TO PROPERLY SET THE TOP LEFT CORNER
				//OF THE SCREEN BEFORE ANYTHING ELSE IS CREATED.
				if (presets.objectType == "Player")
				{
					int halfWidth = devices->GetGraphicsDevice()->getScreenDimensions().x / 2;
					int halfHeight = devices->GetGraphicsDevice()->getScreenDimensions().y / 2;
					// sets the top left corenr of the map
					squarePosition.x = presets.bodyInitializers.position.x*(-1) + halfWidth;
					squarePosition.y = presets.bodyInitializers.position.y*(-1) + halfHeight;
					//Keep track of that corner.
					devices->SetCityCorner(squarePosition);
					//puts the player in the middle of the screen.
					presets.bodyInitializers.position.x = halfWidth;
					presets.bodyInitializers.position.y = halfHeight;
				}
				//Create a pointer to a new object and initialize it.
			
				//add new object
				objects.push_back(objectFactory->Create(presets));

				//mark the exit
				//TODO::Make exits more generic. Perhaps an exit component?
				if (presets.objectType == "Trapdoor") devices->setExit(objects.back().get());

				//make sure presests is ready for loading the level
				presets.bodyInitializers.position = squarePosition;

			}
			else
			{
				//reset ghost direction map
				presets.gDirection.clear();
				presets.bodyInitializers.angle = 0;
				//***********TOP WALL**********************
				std::string top{ squareElement->Attribute("top") };
				if (top == "wall")
				{
					presets.objectType = "HWall";
				}
				else if (top == "door")
				{
					presets.objectType = "HDoor";
				}
				else if (top == "ghost")
				{
					presets.objectType = "HWall";
					bool direction;
					squareElement->QueryBoolAttribute("gN", &direction);
					presets.gDirection[Direction::N] = direction;
					squareElement->QueryBoolAttribute("gS", &direction);
					presets.gDirection[Direction::S] = direction;
				}
				else if (top == "none")
				{
					presets.objectType = "TopFill";
				}

				//Create a pointer to a new object and initialize it.
				
				//add new object
				objects.emplace_back(objectFactory->Create(presets));
				//***************************************

				//***********LEFT WALL**********************
				//left wall starts below the top wall which is 10 pixels tall.
				presets.gDirection.clear();
				presets.bodyInitializers.position.y += 10;
			
				std::string left{ squareElement->Attribute("left") };
				//set left wall
				if (left == "wall")
				{
					presets.objectType = "VWall";
				}
				else if (left == "door")
				{
					presets.objectType = "VDoor";
				}
				else if (left == "ghost")
				{
					presets.objectType = "VWall";
					bool direction;
					squareElement->QueryBoolAttribute("gE", &direction);
					presets.gDirection[Direction::E] = direction;
					squareElement->QueryBoolAttribute("gW", &direction);
					presets.gDirection[Direction::W] = direction;
				}

				//if there is a left wall, create it
				if (left != "none")
				{
					//Create a pointer to a new object and initialize it.
					
					//add new object
					objects.emplace_back(objectFactory->Create(presets));
				}
				//***************************************

				//***********FLOOR**********************
				//we are already 10 down, now move 10 over to position the floor tile not covering
				//any other tiles.
				presets.gDirection.clear();
				presets.bodyInitializers.position.x += 10;
				std::string floor{ squareElement->Attribute("floor") };
				if (floor == "wall")
				{
					presets.objectType = "WallFloor";
				}
				else if (floor == "ghost")
				{
					presets.objectType = "WallFloor";
					presets.gDirection[Direction::N] = true;
					presets.gDirection[Direction::E] = true;
					presets.gDirection[Direction::S] = true;
					presets.gDirection[Direction::W] = true;
				}

				//if there is a floor, create it.
				if (floor != "none")
				{
					//Create a pointer to a new object and initialize it.
					
					//add new object
					objects.emplace_back(objectFactory->Create(presets));
				}
				//***************************************
				//move x to next square to the right (we already moved ten in the code above).
				presets.bodyInitializers.position.x += squareDimension - 10;
				//move back up to top of square
				presets.bodyInitializers.position.y -= 10;

			}
			//Next square to the right
			squareElement = squareElement->NextSiblingElement();
		} while (squareElement);

		//get next row
		rowElement = rowElement->NextSiblingElement();
		//move x to beginning of row
		presets.bodyInitializers.position.x = devices->GetCityCorner().x;
		//only move a row down if we are doing rows, not extras.
		if (label != "Extras") presets.bodyInitializers.position.y += 110;
	} while (rowElement);

	devices->GetSoundDevice()->SetBackground(Locations::sorpigal);

	return true;
}

//**************************************
//Runs the game, including regulation of the game timer,
//and calling the update & draw methods.
bool Game::run()
//**************************************
{
	FrameCounter::incrementFrame();
	//check to see if we have quit;
	if (devices->GetInputDevice()->isPressed(InputDevice::Inputs::quit))
	{
		return false;
	}

	//here's where we load the basement if the variable was set.
	//this needs to be abstracted a bit. . .
	if (devices->GetLoadBasement())
	{
		loadLevel("./Assets/Config/BasementLevel.xml", "./Assets/Config/BasementAssets.xml");
	}
	//Poll for new events
	devices->GetInputDevice()->update();

	
	

	//Start Frame Timer
	frameTimer->start();
	devices->GetGraphicsDevice()->begin();
	
	update();
	
	devices -> GetGraphicsDevice() -> drawOverlays();
		
	if (debug) devices->GetPhysicsDevice()->debugDraw(); 
		
	devices -> GetGraphicsDevice() -> present();

	//pauses until proper refresh time has passed.
	frameTimer->fpsRegulate();
	return true;
}
//**************************************
//call's the view's update method
//Call's each object's update method in the object vector.
void Game::update()
//**************************************
{

	//update the physics world
	devices->GetPhysicsDevice()->Update(1.0f / devices->GetFPS());

	std::vector<std::unique_ptr<GameObject>>::iterator objectIter;

	//clean out dead objects
	for (objectIter = objects.begin(); objectIter != objects.end(); objectIter++)
	{
		if (*objectIter == nullptr)
		{
			objectIter = objects.erase(objectIter);
			objectIter--;
		}
		else
		{
			//check for health component
			HealthComponent* compHealth{ (*objectIter)->getComponent<HealthComponent>() };
			InventoryComponent* compInventory{ (*objectIter)->getComponent<InventoryComponent>() };
			if (compHealth != nullptr)
			{
				if (compHealth->isDead())
				{

					objectIter = objects.erase(objectIter);
					objectIter--;
				}
				//if it got picked up. . 
			}
		}
	}


	//add any objects created in the previous iteration
	if (!newObjects.empty())
	{
		for (auto& object : newObjects)
		{
			objects.push_back(std::move(object));
		}
		newObjects.clear();
	}

	//Update objects.
	for (auto& object : objects)
	{
		//run update method for the object
		if (object != nullptr)
		{
			std::unique_ptr<GameObject> temp{ object->update(objects) };
			//if it returned an object, add it to the list to be added.
			if (temp != nullptr)
			{
				newObjects.push_back(std::move(temp));
			}
		}


	}
}

//**************************************
//Starts the graphic deivce, 
//Graphic Device draws the sprites
//Then calls the graphic device's present method to show the frame.
//bool Game::run()
////**************************************
//{
//	devices -> GetGraphicsDevice() -> begin();	
//	devices -> GetGraphicsDevice() -> run();
//	
//	if (debug) devices->GetPhysicsDevice()->getWorld()->DebugDraw(); //-> DrawDebugData();
//	
//	devices -> GetGraphicsDevice() -> present();
//	return true;
//}

//**************************************
//Clears the object vector and ResourceManager in
//preparation of loading a new level.
void Game::reset()
//**************************************
{
		objects.clear();
		devices = nullptr;
}