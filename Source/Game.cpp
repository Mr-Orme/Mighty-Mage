
#include <algorithm>
#include "Game.h"
#include "ComponentsList.h"
#include "FrameCounter.h"
#include "Timer.h"
#include "Definitions.h"
#include "GameObject.h"
#include "Initializers.h"

#include "GraphicsDevice.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"
#include "ObjectLibrary.h"
#include "NoticesAssetLibrary.h"
#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "LevelConfigLibrary.h"
#include "View.h"

Game::Game()
{
	std::string levelFile{ "./Assets/Config/Areas.xml" };
	loadLevelLibrary(levelFile);
	loadLevel(Levels::sorpigal);
}



Game::~Game()
{}



bool Game::loadLevel(Levels toLoad)
{
	//When we are loading a level from within the game, the starting position and the direciton for the player are
	//stored in devices. Otherwise, we use the values loaded from the file.
	std::optional<Vector2D> playerStart{ std::nullopt };
	std::optional<Direction> playerDirection{ std::nullopt };
	if (devices)//devices is nullptr on game start!
	{
		auto [start, direction] = devices->getPlayerStart();
		playerStart = start;
		playerDirection = direction;
	}

	reset();

	auto [levelConfig, assetConfigFile] = levelLibrary->search(toLoad);
	
	devices = std::make_unique<ResourceManager>(screenDimensions, assetConfigFile);
	frameTimer = std::make_unique<Timer>(devices->FPS);
	
	parseLevelXML(levelConfig, playerStart, playerDirection);	

	devices->getSoundDevice()->SetBackground(toLoad);

	return true;
}

//**************************************
//Runs the game, including regulation of the game timer,
//and calling the update & draw methods.
bool Game::run()
//**************************************
{
	frameTimer->start();
	if (auto level{ devices->level2Load() }; level != Levels::none)
	{
		loadLevel(level);
	}
	
	
	if (devices->getInputDevice()->isPressed(Inputs::quit))
	{
		return false;
	}
	devices->getInputDevice()->update();//HACK::This HAS to be here otherwise pickup sound does not play. Don't know why???
	devices->getGraphicsDevice()->begin();
	
	update();
	
	devices -> getGraphicsDevice() -> drawOverlays();
	if (debug) devices->getPhysicsDevice()->debugDraw(); 
	devices -> getGraphicsDevice() -> present();

	frameTimer->fpsRegulate();
	FrameCounter::incrementFrame();
	return true;
}
//**************************************
//call's the view's update method
//Call's each object's update method in the object vector.
bool Game::update()
//**************************************
{
	devices->getPhysicsDevice()->Update(1.0f / devices->FPS);

	objects.erase(std::remove_if(objects.begin(), objects.end(), [](std::unique_ptr<GameObject>& object)
		{
			if (!object) return true;
			
			if (auto compHealth{ object->getComponent<HealthComponent>() }; compHealth)
			{
				return compHealth->isDead();
			}
			return false;
		}), objects.end());


	
	std::move(newObjects.begin(), newObjects.end(), std::back_inserter(objects));
	
	for (auto& object : objects)
	{
		if (object)
		{
			if (auto temp{ object->update(objects) }; temp)
			{
				newObjects.push_back(std::move(temp));
			}
		}
	}
	return true;
}

void Game::loadLevelLibrary(std::string levelFile)
{
	levelLibrary = std::make_unique<LevelConfigLibrary>();
	tinyxml2::XMLDocument currentLevel;

	if (!currentLevel.LoadFile(levelFile.c_str()) == tinyxml2::XML_SUCCESS)
	{
		std::cout << "error with Areas.xml file";
	};
	tinyxml2::XMLElement* lRoot{ currentLevel.FirstChildElement() };
	tinyxml2::XMLElement* level{ lRoot->FirstChildElement() };
	while (level)
	{
		int enumValue{};
		level->QueryIntAttribute("enum", &enumValue);
		std::string path{ level->Attribute("layout") };
		std::string assets{ level->Attribute("assets") };
		levelLibrary->addAsset((Levels)enumValue, "./Assets/Config/" + path, "./Assets/Config/" + assets);
		level = level->NextSiblingElement();
	}
}

ObjectFactoryPresets Game::loadExtras(tinyxml2::XMLElement* squareElement, std::optional<Vector2D> playerStart, std::optional<Direction> playerDirection)
{
	ObjectFactoryPresets presets;
	presets.id.name = squareElement->Value();

	float tempX{};
	float tempY{};
	squareElement->QueryFloatAttribute("x", &tempX);
	squareElement->QueryFloatAttribute("y", &tempY);
	presets.bodyInitializers.position = devices->square2Pixel(tempX, tempY);

	squareElement->QueryIntAttribute("angle", &presets.bodyInitializers.angle);
	//TODO::These need to be moved to the constructor
	if (presets.id.name == "Player")
	{
		if (playerStart)
		{
			presets.bodyInitializers.position = devices->square2Pixel(*playerStart, { 50, 50 });
		}
		if (playerDirection) presets.bodyInitializers.angle = (int)*playerDirection;//TODO::Move the casting to an int to physics device
		
		// center the view around player
		
		Vector2D halfScreen{ devices->getGraphicsDevice()->getScreenDimensions() / 2 };
		devices->getGraphicsDevice()->getView()->setPosition(presets.bodyInitializers.position - halfScreen);

	}
	
	return presets;
	
}
void Game::loadWalls(tinyxml2::XMLElement* wallElement, Vector2D square)
{
	ObjectFactoryPresets presets;

	std::string wallDirections{ wallElement->Attribute("directions") };
	auto wallType{ wallElement->Attribute("type") };

	for (int i{ 0 }; i < wallDirections.size(); i++)
	{
		presets.bodyInitializers.position = devices->square2Pixel(square);
		presets.gDirection.clear();
		switch (wallDirections.at(i))
		{
			case 'N':
				presets.id.name = "HWall";//TODO::use rotation to set horizontal/vertical instead of separate images.
				presets.bodyInitializers.position.y -= 5;
				presets.bodyInitializers.position.x -= 5;
				if (wallType)
				{
					
					switch (wallType[i])//char* so have to use []
					{
					case 'D':
						presets.id.name = "HDoor";
						break;
					case 'I':
						presets.gDirection[Direction::S] = true;
						break;
					case 'O':
						presets.gDirection[Direction::N] = true;
						break;
					case 'B':
						presets.gDirection[Direction::N] = true;
						presets.gDirection[Direction::S] = true;
						break;
					}
				}
				break;
			case 'E':
				presets.id.name = "TopFill";
				presets.bodyInitializers.position = devices->square2Pixel(square) + Vector2D{ 105, -5 };
				objects.emplace_back(devices->getObjectFactory()->Create(presets, devices.get()));

				presets.id.name = "VWall";
				presets.bodyInitializers.position = devices->square2Pixel(square) + Vector2D{ 105, 5 };
				if (wallType)
				{
					
					switch (wallType[i])//char* so have to use []
					{
					case 'D':
						presets.id.name = "VDoor";
						break;
					case 'I':
						presets.gDirection[Direction::W] = true;
						break;
					case 'O':
						presets.gDirection[Direction::E] = true;
						break;
					case 'B':
						presets.gDirection[Direction::E] = true;
						presets.gDirection[Direction::W] = true;
						break;
					}
				}
				break;
			case 'S':
				presets.id.name = "HWall";
				presets.bodyInitializers.position.y += 105;
				presets.bodyInitializers.position.x -= 5;
				if (wallType)
				{
					
					switch (wallType[i])//char* so have to use []
					{
					case 'D':
						presets.id.name = "HDoor";
						break;
					case 'I':
						presets.gDirection[Direction::N] = true;
						break;
					case 'O':
						presets.gDirection[Direction::S] = true;
						break;
					case 'B':
						presets.gDirection[Direction::N] = true;
						presets.gDirection[Direction::S] = true;
						break;
					}
				}
				break;
			case 'W':
				presets.id.name = "VWall";
				presets.bodyInitializers.position.x -= 5;
				presets.bodyInitializers.position.y += 5;
				if (wallType)
				{
					
					switch (wallType[i])//char* so have to use []
					{
					case 'D':
						presets.id.name = "VDoor";
						break;
					case 'I':
						presets.gDirection[Direction::E] = true;
						break;
					case 'O':
						presets.gDirection[Direction::W] = true;
						break;
					case 'B':
						presets.gDirection[Direction::E] = true;
						presets.gDirection[Direction::W] = true;
						break;
					}
				}
				break;
		}
		objects.emplace_back(devices->getObjectFactory()->Create(presets, devices.get()));
	}
}


void Game::loadFloor(Vector2D square)
{
	ObjectFactoryPresets presets;

	presets.gDirection.clear();
	presets.bodyInitializers.position = devices->square2Pixel(square);
	presets.bodyInitializers.position += 5;
	presets.id.name = "WallFloor";
	presets.gDirection[Direction::N] = true;
	presets.gDirection[Direction::E] = true;
	presets.gDirection[Direction::S] = true;
	presets.gDirection[Direction::W] = true;

	objects.emplace_back(devices->getObjectFactory()->Create(presets, devices.get()));
}

bool Game::parseLevelXML(std::string levelConfig, std::optional<Vector2D> playerStart, std::optional<Direction> playerDirection)
{
	ObjectFactory* objectFactory{ devices->getObjectFactory() };

	tinyxml2::XMLDocument currentLevel;
	if (!currentLevel.LoadFile(levelConfig.c_str()) == tinyxml2::XML_SUCCESS) { return false; };
	
	tinyxml2::XMLElement* levelRoot{ currentLevel.FirstChildElement() };
	
	int iLevel;
	levelRoot->QueryIntAttribute("level", &iLevel);
	devices->setLevel(static_cast<Levels>(iLevel));

	tinyxml2::XMLElement* rowElement = levelRoot->FirstChildElement();

	Vector2D square{ 0,0 };
	while (rowElement)
	{
		rowElement->QueryIntAttribute("id", &square.y);
		auto squareElement{ rowElement->FirstChildElement() };
		std::string label = rowElement->Value();

		while (squareElement)
		{
			if (label == "Extras") // not walls or doors
			{
				objects.push_back(objectFactory->Create(loadExtras(squareElement, playerStart, playerDirection), devices.get()));
			}
			else
			{
				
				squareElement->QueryIntAttribute("id", &square.x);
				auto objectElement{ squareElement->FirstChildElement() };
				while (objectElement)
				{
					std::string elementType{ objectElement->Value() };
					if (elementType == "Wall")
					{
						loadWalls(objectElement, square);
					}
					else if (elementType == "Floor")
					{
						loadFloor(square);
					}
					
					objectElement = objectElement->NextSiblingElement();
				}
				ObjectFactoryPresets presets;
				presets.id.name = "TopFill";
				presets.bodyInitializers.position = devices->square2Pixel(square) + Vector2D{ -5, -5 };
				objects.emplace_back(devices->getObjectFactory()->Create(presets, devices.get()));
			}
			squareElement = squareElement->NextSiblingElement();
		}
		rowElement = rowElement->NextSiblingElement();
	}
	return true;
}


//**************************************
//Clears the object vector and ResourceManager in
//preparation of loading a new level.
void Game::reset()
//**************************************
{
		objects.clear();
		devices = nullptr;
}