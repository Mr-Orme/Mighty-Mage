
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

std::optional<ObjectFactoryPresets> Game::loadLeftWall(tinyxml2::XMLElement* squareElement, Vector2D square)
{
	ObjectFactoryPresets presets;
	presets.bodyInitializers.position = devices->square2Pixel(square);
	presets.bodyInitializers.position.y += 10;

	std::string left{ squareElement->Attribute("left") };
	if (left == "wall")
	{
		presets.id.name = "VWall";
	}
	else if (left == "door")
	{
		presets.id.name = "VDoor";
	}
	else if (left == "ghost")
	{
		presets.id.name = "VWall";
		squareElement->QueryBoolAttribute("gE", &presets.gDirection[Direction::E]);
		squareElement->QueryBoolAttribute("gW", &presets.gDirection[Direction::W]);
	}
	if (left != "none")
	{
		return presets;
	}
	return std::nullopt;
	
}

ObjectFactoryPresets Game::loadTopWall(tinyxml2::XMLElement* squareElement, Vector2D square)
{
	ObjectFactoryPresets presets;
	presets.bodyInitializers.position = devices->square2Pixel(square);
	
	std::string top{ squareElement->Attribute("top") };
	if (top == "wall")
	{
		presets.id.name = "HWall";
	}
	else if (top == "door")
	{
		presets.id.name = "HDoor";
	}
	else if (top == "ghost")
	{
		presets.id.name = "HWall";
		squareElement->QueryBoolAttribute("gN", &presets.gDirection[Direction::N]);
		squareElement->QueryBoolAttribute("gS", &presets.gDirection[Direction::S]);
	}
	else if (top == "none")
	{
		presets.id.name = "TopFill";
	}

	return presets;
}

std::optional<ObjectFactoryPresets> Game::loadFloor(tinyxml2::XMLElement* squareElement, Vector2D square)
{
	ObjectFactoryPresets presets;
	presets.gDirection.clear();
	presets.bodyInitializers.position = devices->square2Pixel(square);
	presets.bodyInitializers.position += 10;//HACK::This is what puts the top and left in the wrong square!
	std::string floor{ squareElement->Attribute("floor") };
	if (floor == "wall")
	{
		presets.id.name = "WallFloor";
	}
	else if (floor == "ghost")
	{
		presets.id.name = "WallFloor";
		presets.gDirection[Direction::N] = true;
		presets.gDirection[Direction::E] = true;
		presets.gDirection[Direction::S] = true;
		presets.gDirection[Direction::W] = true;
	}

	if (floor != "none")
	{
		return presets;
	}

	return std::nullopt;
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
		tinyxml2::XMLElement* squareElement = rowElement->FirstChildElement();
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

				objects.emplace_back(objectFactory->Create(loadTopWall(squareElement, square), devices.get()));

				if (auto presets{ loadLeftWall(squareElement, square) }; presets)
				{
					objects.emplace_back(objectFactory->Create(*presets, devices.get()));
				}
				if (auto presets{ loadFloor(squareElement, square) }; presets)
				{
					objects.emplace_back(objectFactory->Create(*presets, devices.get()));
				}
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