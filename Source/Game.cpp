#include "Game.h"
#include "ComponentsList.h"
//**************************************
//Initiallizes variables to Null values.
Game::Game()
//**************************************
{
	devices = NULL;
	debug = false;
}

Game::~Game()
{
	if(devices)
	{
		devices -> Shutdown();
		devices = NULL;
	}
}

//**************************************
//Initializes Game & Art Asset Libraries.
bool Game::Initialize()
//**************************************
{
	//========================================
	//Base Game Constants
	//========================================
	SCREEN_WIDTH = 800;
	SCREEN_HEIGHT = 600;
	
	
	
	return true;
}

//**************************************
//Takes a string corresponding to an XML file
//containing level config data and loads the objects
//into the objects vector.
//Also creates a new view object for the level.
bool Game::LoadLevel(std::string levelConfig, std::string assetConfigFile)
//**************************************
{

	Reset();
	//========================================
	//Construct Device Manager
	//========================================
	devices = std::make_shared<ResourceManager>();
	devices->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, assetConfigFile);
	this->devices = devices;
	



	///Get a few things ready
	GAME_OBJECTFACTORY_PRESETS presets;
	presets.devices = devices;
	ObjectFactory* objectFactory = devices->GetObjectFactory();

	//========================================
	//load the files
	//========================================
	TiXmlDocument currentLevel(levelConfig);
	//if it does not load, the level or the physics did not load.
	if (!currentLevel.LoadFile()){ return false; };
	//create a root variable and set to the first element "Level"
	TiXmlElement* lRoot = currentLevel.FirstChildElement();;
	//record the level;
	int iLevel;
	lRoot->QueryIntAttribute("level", &iLevel);
	devices->SetLevel(static_cast<GAME_LEVEL>(iLevel));



	//get the first child element "Row"
	TiXmlElement* rowElement = lRoot->FirstChildElement();


	//Size, in pixels, of one square in the game space
	GAME_INT squareDimension = 110;
	presets.position.x = 0;
	presets.position.y = 0;
	//keeps track of the game square we are currently on.
	GAME_VEC squarePosition = { presets.position.x, presets.position.y };

	//========================================
	//Add level elements to object array
	//========================================
	do
	{
		//get's the first square;
		TiXmlElement* squareElement = rowElement->FirstChildElement();
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
				presets.position.x = (temp *squareDimension) + squarePosition.x;

				//grab the y value
				squareElement->QueryFloatAttribute("y", &temp);

				//separate into fraction and integerparts;
				float fractPart, intPart;
				fractPart = modff(temp, &intPart);
				//The SDL top starts at 0, Our map is reversed, so we need to subtract the integer part from 15 and add back on the fractinal part.
				presets.position.y = (((15 - intPart) + fractPart) * squareDimension) + squarePosition.y;

				//grab the angle
				squareElement->QueryFloatAttribute("angle", &presets.angle);

				//adjusts where we start the level based on the player's position
				//and puts the player in the middle of the screen.
				//PLAYER MUST BE THE FIRST ITEM IN THE XML IN ORDER TO PROPERLY SET THE TOP LEFT CORNER
				//OF THE SCREEN BEFORE ANYTHING ELSE IS CREATED.
				if (presets.objectType == "Player")
				{
					GAME_INT halfWidth = devices->GetGraphicsDevice()->GetScreenWidth() / 2;
					GAME_INT halfHeight = devices->GetGraphicsDevice()->GetScreenHeight() / 2;
					// sets the top left corenr of the map
					squarePosition.x = presets.position.x*(-1) + halfWidth;
					squarePosition.y = presets.position.y*(-1) + halfHeight;
					//Keep track of that corner.
					devices->SetCityCorner(squarePosition);
					//puts the player in the middle of the screen.
					presets.position.x = halfWidth;
					presets.position.y = halfHeight;
				}
				//Create a pointer to a new object and initialize it.
				std::shared_ptr<GameObject> newObject = objectFactory->Create(presets);
				//add new object
				objects.push_back(newObject);

				//mark the exit
				if (presets.objectType == "Trapdoor") devices->GetGraphicsDevice()->SetExit(newObject);

				//make sure presests is ready for loading the level
				presets.position = squarePosition;

			}
			else
			{
				//reset ghost direction map
				presets.gDirection.clear();
				presets.angle = 0;
				//***********TOP WALL**********************
				std::string top;
				//WHat type of wall is on top
				squareElement->QueryStringAttribute("top", &top);
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
					presets.gDirection[N] = direction;
					squareElement->QueryBoolAttribute("gS", &direction);
					presets.gDirection[S] = direction;
				}
				else if (top == "none")
				{
					presets.objectType = "TopFill";
				}

				//Create a pointer to a new object and initialize it.
				std::shared_ptr<GameObject> newObject = objectFactory->Create(presets);
				//add new object
				objects.push_back(newObject);
				//***************************************

				//***********LEFT WALL**********************
				//left wall starts below the top wall which is 10 pixels tall.
				presets.gDirection.clear();
				presets.position.y += 10;
				//grab left wall
				std::string left;
				squareElement->QueryStringAttribute("left", &left);
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
					presets.gDirection[E] = direction;
					squareElement->QueryBoolAttribute("gW", &direction);
					presets.gDirection[W] = direction;
				}

				//if there is a left wall, create it
				if (left != "none")
				{
					//Create a pointer to a new object and initialize it.
					std::shared_ptr<GameObject> newObject = objectFactory->Create(presets);
					//add new object
					objects.push_back(newObject);
				}
				//***************************************

				//***********FLOOR**********************
				//we are already 10 down, now move 10 over to position the floor tile not covering
				//any other tiles.
				presets.gDirection.clear();
				presets.position.x += 10;
				std::string floor;
				squareElement->QueryStringAttribute("floor", &floor);
				if (floor == "wall")
				{
					presets.objectType = "WallFloor";
				}
				else if (floor == "ghost")
				{
					presets.objectType = "WallFloor";
					presets.gDirection[N] = true;
					presets.gDirection[E] = true;
					presets.gDirection[S] = true;
					presets.gDirection[W] = true;
				}

				//if there is a floor, create it.
				if (floor != "none")
				{
					//Create a pointer to a new object and initialize it.
					std::shared_ptr<GameObject> newObject = objectFactory->Create(presets);
					//add new object
					objects.push_back(newObject);
				}
				//***************************************
				//move x to next square to the right (we already moved ten in the code above).
				presets.position.x += squareDimension - 10;
				//move back up to top of square
				presets.position.y -= 10;

			}
			//Next square to the right
			squareElement = squareElement->NextSiblingElement();
		} while (squareElement);

		//get next row
		rowElement = rowElement->NextSiblingElement();
		//move x to beginning of row
		presets.position.x = devices->GetCityCorner().x;
		//only move a row down if we are doing rows, not extras.
		if (label != "Extras") presets.position.y += 110;
	} while (rowElement);

	//reverse the order of the sprites so the player is on top of everything.
	devices->GetGraphicsDevice()->ReverseOrder();

	//start background music

	devices->GetSoundDevice()->SetBackground("main");



	//Successfully loaded level
	return true;
}

//**************************************
//Runs the game, including regulation of the game timer,
//and calling the update & draw methods.
bool Game::Run()
//**************************************
{

	//check to see if we have quit;
	if (devices->GetInputDevice()->GetEvent(GAME_QUIT) == true)
	{
		return false;
	}

	//here's where we load the basement if the variable was set.
	//this needs to be abstracted a bit. . .
	if (devices->GetLoadBasement())
	{
		LoadLevel("./Assets/Config/BasementLevel.xml", "./Assets/Config/BasementAssets.xml");
	}
	//Poll for new events
	devices->GetInputDevice()->Update();

	//Construct Frame Timer
	Timer* frameRate = new Timer();
	if (!frameRate->Initialize(devices->GetFPS()))
	{
		printf("Frame Timer could not intialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//Start Frame Timer
	frameRate->start();

	Update();
	Draw();

	//pauses until proper refresh time has passed.
	frameRate->fpsRegulate();
	return true;
}
//**************************************
//call's the view's update method
//Call's each object's update method in the object vector.
void Game::Update()
//**************************************
{

	//update the physics world
	devices->GetPhysicsDevice()->Update(1.0f / devices->GetFPS());

	std::vector<std::shared_ptr<GameObject>>::iterator objectIter;

	//clean out dead objects
	for (objectIter = objects.begin(); objectIter != objects.end(); objectIter++)
	{
		//check for health component
		std::shared_ptr<HealthComponent> compHealth = (*objectIter)->GetComponent<HealthComponent>();
		std::shared_ptr<InventoryComponent> compInventory = (*objectIter)->GetComponent<InventoryComponent>();
		if (compHealth != NULL)
		{
			//if it is dead
			if (compHealth->GetIsDead())
			{
				//close off the componenets.
				(*objectIter)->removeComponents();

				//remove object from the vector.
				objects.erase(objectIter);
				//back up to previous item because this one was deleted.
				objectIter--;
			}
			//if it got picked up. . 
			else if (compInventory != NULL && compInventory->GetPickedUp())
			{
				//remove the sprite from the automatic draw list
				devices->GetGraphicsDevice()->RemoveSpriteRenderer((*objectIter)->GetComponent<RendererComponent>().get());
				//stop the physics on it
				devices->GetPhysicsDevice()->SetStopPhysics((*objectIter).get());
				//remove object from the vector.
				objectIter = objects.erase(objectIter);


			}
		}
	}


	//add any objects created in the previous iteration
	if (!newObjects.empty())
	{
		objects.insert(objects.end(), newObjects.begin(), newObjects.end());
		newObjects.clear();
	}

	//Update objects.
	for (auto object : objects)
	{
		//run update method for the object
		std::shared_ptr<GameObject> temp = object->Update();
		//if it returned an object, add it to the list to be added.
		if (temp != NULL)
		{
			newObjects.push_back(temp);
		}


	}
}

//**************************************
//Starts the graphic deivce, 
//Graphic Device draws the sprites
//Then calls the graphic device's present method to show the frame.
void Game::Draw()
//**************************************
{
	devices -> GetGraphicsDevice() -> Begin();	
	devices -> GetGraphicsDevice() -> Draw();
	
	if(debug) devices -> GetPhysicsDevice() -> getWorld() -> DrawDebugData();
	
	devices -> GetGraphicsDevice() -> Present();
}

//**************************************
//Clears the object vector, world, and view object in 
//preparation of loading a new level.
void Game::Reset()
//**************************************

{
	//if objects is not empty
	if (!objects.empty())
	{
		//for every object in objects
		for (const auto object : objects)
		{
			//remove it from the physics world
			devices->GetPhysicsDevice()->RemoveObject(object.get());
		}
		//clear the vector
		objects.clear();
	}
	//kill old Resource Manager;
	if (devices)	devices->Shutdown();
	devices = NULL;


}