
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <optional>

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

#include "tinyxml2.h"
#include "Vector2D.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"

Game::Game()
	:levelLibrary{ std::make_unique<LevelConfigLibrary>() }
{
	std::string levelFile{ "./Assets/Config/Areas.xml" };
	tinyxml2::XMLDocument currentLevel;

	if (auto loaded{ currentLevel.LoadFile(levelFile.c_str() )}; loaded != tinyxml2::XML_SUCCESS)
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
		levelLibrary->addAsset(static_cast<Levels>(enumValue), "./Assets/Config/" + path, "./Assets/Config/" + assets);
		level = level->NextSiblingElement();
	}
	loadLevel(Levels::sorpigal);
}



Game::~Game()
{}


//**************************************
//Takes a string corresponding to an XML file
//containing level config data and loads the objects
//into the objects vector.
//Also creates a new view object for the level.
bool Game::loadLevel(Levels toLoad)
//**************************************
{
	std::optional<Vector2D> playerStart{ std::nullopt };
	std::optional<Direction> playerDirection{ std::nullopt };
	if (devices)
	{
		auto [start, direction] = devices->getPlayerStart();
		playerStart = start;
		playerDirection = direction;
	}

	reset();

	auto [levelConfig, assetConfigFile] = levelLibrary->search(toLoad);

	devices = std::make_unique<ResourceManager>(screenDimensions, assetConfigFile);
	frameTimer = std::make_unique<Timer>(devices->FPS);

	//TODO::pass in devices instead of having it in presets! This will simplify the load helper functions!
	ObjectFactory* objectFactory{ devices->getObjectFactory() };


	//========================================
	//load the files
	//========================================

	tinyxml2::XMLDocument currentLevel;
	if (auto loaded{ currentLevel.LoadFile(levelConfig.c_str()) }; loaded != tinyxml2::XML_SUCCESS)
	{ 
		return false; 
	}

	tinyxml2::XMLElement* levelRoot{ currentLevel.FirstChildElement() };
	int iLevel;
	levelRoot->QueryIntAttribute("level", &iLevel);
	devices->setLevel(static_cast<Levels>(iLevel));

	tinyxml2::XMLElement* rowElement{ levelRoot->FirstChildElement() };

	Vector2D square{ 0,0 };
	while (rowElement)
	{
		rowElement->QueryIntAttribute("id", &square.y);
		tinyxml2::XMLElement* squareElement{ rowElement->FirstChildElement() };
		std::string label{ rowElement->Value() };

		while (squareElement)
		{
			if (label == "Extras") // not walls or doors
			{
				objects.push_back(objectFactory->Create(loadExtras(squareElement, devices.get(), playerStart, playerDirection)));
			}
			else
			{
				squareElement->QueryIntAttribute("id", &square.x);

				objects.emplace_back(objectFactory->Create(loadTopWall(squareElement, square, devices.get())));

				if (auto presets{ loadLeftWall(squareElement, square, devices.get()) }; presets)
				{
					objects.emplace_back(objectFactory->Create(*presets));
				}
				if (auto presets{ loadFloor(squareElement, square, devices.get()) }; presets)
				{
					objects.emplace_back(objectFactory->Create(*presets));
				}
			}
			squareElement = squareElement->NextSiblingElement();
		}
		rowElement = rowElement->NextSiblingElement();
	}

	devices->getSoundDevice()->SetBackground(Locations::sorpigal);

	return true;
}

//**************************************
//Runs the game, including regulation of the game timer,
//and calling the update & draw methods.
bool Game::run()
//**************************************
{
	//TODO::modify level2Load to also return starting position!
	if (auto level{ devices->level2Load() }; level != Levels::none)
	{
		loadLevel(level);
	}
	FrameCounter::incrementFrame();

	if (devices->getInputDevice()->isPressed(Inputs::quit))
	{
		return false;
	}

	devices->getInputDevice()->update();

	frameTimer->start();
	devices->getGraphicsDevice()->begin();

	update();

	devices->getGraphicsDevice()->drawOverlays();

	//if (debug) devices->getPhysicsDevice()->debugDraw();

	devices->getGraphicsDevice()->present();

	//pauses until proper refresh time has passed.
	frameTimer->fpsRegulate();
	return true;
}
//**************************************
//call's the view's update method
//Call's each object's update method in the object vector.
bool Game::update()
//**************************************
{


	//update the physics world
	devices->getPhysicsDevice()->Update(1.0f / devices->FPS);

	

	//clean out dead objects
	//TODO::update to algorithm!
	for (auto objectIter{ objects.begin() }; objectIter != objects.end(); objectIter++)
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


	for (auto& object : objects)
	{
		if (object != nullptr)
		{
			auto temp{ object->update(objects) };
			if (temp != nullptr)
			{
				newObjects.push_back(std::move(temp));
			}
		}
	}
	return true;
}

ObjectFactoryPresets Game::loadExtras(tinyxml2::XMLElement* squareElement, ResourceManager* devices, std::optional<Vector2D> playerStart, std::optional<Direction> playerDirection)
{
	ObjectFactoryPresets presets;
	presets.devices = devices;
	presets.objectType = squareElement->Value();

	float tempX{};
	float tempY{};
	squareElement->QueryFloatAttribute("x", &tempX);
	squareElement->QueryFloatAttribute("y", &tempY);
	presets.bodyInitializers.position = devices->square2Pixel(tempX, tempY);

	squareElement->QueryIntAttribute("angle", &presets.bodyInitializers.angle);
	/*TODO::pass in a position for player
	* Need to modify level toLoad to include position. Should be returned from trigger.
	*/
	if (presets.objectType == "Player")
	{
		if (playerStart)
		{
			presets.bodyInitializers.position = devices->square2Pixel(*playerStart, { 50, 50 });
		}
		if (playerDirection)
		{
			presets.bodyInitializers.angle = static_cast<int>(*playerDirection);
		}
		// center the view around player
		Vector2D halfScreen{ devices->getGraphicsDevice()->getScreenDimensions() / 2 };
		devices->getGraphicsDevice()->getView()->setPosition(presets.bodyInitializers.position - halfScreen);

	}
	else if (presets.objectType == "Trigger")
	{
		//size the trigger so it's near the proper edge of the square.
		switch ((Direction)presets.bodyInitializers.angle)
		{
		case Direction::N:
			presets.bodyInitializers.dimensions = { devices->pixelsPerSquare, static_cast<int>(devices->pixelsPerSquare * 0.2f) };
			break;
		case Direction::E:
			presets.bodyInitializers.dimensions = { static_cast<int>(devices->pixelsPerSquare * 0.2f, devices->pixelsPerSquare) };
			presets.bodyInitializers.position.x += devices->pixelsPerSquare - presets.bodyInitializers.dimensions.x;
			break;
		case Direction::S:
			presets.bodyInitializers.dimensions = { devices->pixelsPerSquare, static_cast<int>(devices->pixelsPerSquare * 0.2f) };
			presets.bodyInitializers.position.y += devices->pixelsPerSquare - presets.bodyInitializers.dimensions.y;
			break;
		case Direction::W:
			presets.bodyInitializers.dimensions = { static_cast<int>(devices->pixelsPerSquare * 0.2f, devices->pixelsPerSquare) };
			break;
		default:
			break;
		}
		squareElement->QueryIntAttribute("type", &presets.triggerInitializers.name);
		if (presets.triggerInitializers.name == static_cast<int>(TriggerComponent::Type::exits))
		{
			int level{};
			squareElement->QueryIntAttribute("area", &level);
			presets.triggerInitializers.exitTo = static_cast<Levels>(level);
			presets.triggerInitializers.message = squareElement->Attribute("message");
			squareElement->QueryIntAttribute("playerAngle", &presets.triggerInitializers.playerAngle);
			squareElement->QueryIntAttribute("playerX", &presets.triggerInitializers.playerLocation.x);
			squareElement->QueryIntAttribute("playerY", &presets.triggerInitializers.playerLocation.y);
		}
	}
	return presets;

}

std::optional<ObjectFactoryPresets> Game::loadLeftWall(tinyxml2::XMLElement* squareElement, Vector2D square, ResourceManager* devices)
{
	ObjectFactoryPresets presets;
	presets.devices = devices;
	presets.bodyInitializers.position = devices->square2Pixel(square);
	presets.bodyInitializers.position.y += 10;

	std::string left{ squareElement->Attribute("left") };
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
	if (left != "none")
	{
		return presets;
	}
	return std::nullopt;

}

ObjectFactoryPresets Game::loadTopWall(tinyxml2::XMLElement* squareElement, Vector2D square, ResourceManager* devices)
{
	ObjectFactoryPresets presets;
	presets.devices = devices;
	presets.bodyInitializers.position = devices->square2Pixel(square);

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

	return presets;
}

std::optional<ObjectFactoryPresets> Game::loadFloor(tinyxml2::XMLElement* squareElement, Vector2D square, ResourceManager* devices)
{
	ObjectFactoryPresets presets;
	presets.devices = devices;
	presets.gDirection.clear();
	presets.bodyInitializers.position = devices->square2Pixel(square);
	presets.bodyInitializers.position += 10;
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

	if (floor != "none")
	{
		return presets;
	}

	return std::nullopt;
}


//**************************************
//Clears the object vector and ResourceManager in
//preparation of loading a new level.
void Game::reset()
//**************************************
{
	
	objects.clear();//This HAS to be first!
	devices = nullptr;

}