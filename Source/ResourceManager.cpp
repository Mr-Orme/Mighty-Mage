
#include <iostream>
#include "ResourceManager.h"
#include "ObjectLibrary.h"
#include "ObjectFactory.h"
#include "Definitions.h"
#include "ComponentsList.h"
#include "Box2DDebugDraw.h"
#include "Initializers.h"
#include "tinyxml2.h"

ResourceManager::ResourceManager(Vector2D screenDimensions, std::string assetPath):
	factory(std::make_unique<ObjectFactory>()),
	iDevice(std::make_unique<InputDevice>()),
	oLibrary(std::make_unique<ObjectLibrary>()),
	nLibrary(std::make_unique<NoticesAssetLibrary>())
{
	//don't load the basement
	loadBasement = false;

	RGBA fontColor{ 0, 0, 0, 255 };
	gDevice = std::make_unique<GraphicsDevice>(screenDimensions, "./Assets/Fonts/impact.ttf", 16, fontColor);
	pDevice = std::make_unique<PhysicsDevice>(Vector2D{ 0,0 }, this);

	sLibrary = std::make_unique<SoundAssetLibrary>();
	sDevice = std::make_unique<SoundDevice>(sLibrary.get());

	loadLibraries(assetPath);
}

ResourceManager::~ResourceManager(){
}

GraphicsDevice* ResourceManager::GetGraphicsDevice()
{
	return gDevice.get();
}

bool ResourceManager::isExitSquare(Vector2D currSquare) const
{
	for (auto square : exits)
	{
		//TODO:: overload == for Vector2D
		//TODO:: 14,0 exit square added for main level!
		if (square.x == currSquare.x && square.y == currSquare.y)
		{
			return true;
		}
	}
	return false;
}

void ResourceManager::loadLibraries(std::string assetPath)
{
	
	//TODO::move to library constructors.
	tinyxml2::XMLDocument assetFile;
	if (!assetFile.LoadFile(assetPath.c_str()) == tinyxml2::XML_SUCCESS) { return; };
	tinyxml2::XMLElement* assetType{ assetFile.FirstChildElement()->FirstChildElement() };

	//get first asset
	tinyxml2::XMLElement* asset{ assetType->FirstChildElement() };
	//add each asset into each library.
	
	populateComponentLibrary(asset);

	//*********************Load Notices***************************
	//move to notice section of file
	assetType = assetType->NextSiblingElement();
	//grab first notice
	tinyxml2::XMLElement* notices{ assetType->FirstChildElement() };
	do
	{
		//get information from file TODO:: x and y directly in notice
		Notice notice;
		
		notices->QueryIntAttribute("x", &notice.square.x);
		notices->QueryIntAttribute("y", &notice.square.y);
		std::string direction {notices->Attribute("direction")};
		
		notice.text = notices->GetText();

		if (direction == "N") notice.direction = Direction::N;
		else if (direction == "E") notice.direction = Direction::E;
		else if (direction == "S") notice.direction = Direction::S;
		else if (direction == "W") notice.direction = Direction::W;

		//add it to the library.
		nLibrary->addAsset(notice);

		//get the next notice
		notices = notices->NextSiblingElement();
	} while (notices);
	//***********************************************************

	//*********************Load Sounds***************************
	//move tos ound section of file
	assetType = assetType->NextSiblingElement();
	//grab first sound
	tinyxml2::XMLElement* sounds{ assetType->FirstChildElement() };
	while (sounds)
	{
		//get information from file
		int sound{ };
		sounds->QueryIntAttribute("name", &sound);
		std::string path{ sounds->Attribute("path") };
		bool background;
		sounds->QueryBoolAttribute("background", &background);

		//add to library based on whether it is background music or not.
		if (background)
		{
			sLibrary->addBackgroundMusic((Locations)sound, path);
		}
		else
		{
			float duration{};
			sounds->QueryFloatAttribute("duration", &duration);
			if (duration != 0)
			{
				duration = FPS * duration;
			}
			sLibrary->addSoundEffect((SoundEffect::Event)sound, path, (int)duration);
		}
		sounds = sounds->NextSiblingElement();
	}



	//***********************************************************
}

void ResourceManager::populateComponentLibrary(tinyxml2::XMLElement* asset)
{
	do
	{
		std::string aName{ asset->Attribute("name") };

		ObjectDefinition definition;

		//move to the components of the xml
		tinyxml2::XMLElement* compElement = asset->FirstChildElement();

		//Add each component to the vector
		while (compElement)
		{
			//get the component's name
			std::string currentComponent = compElement->Attribute("name");
			//find the component we just grabbed
			if (currentComponent == "Renderer")
			{
				definition.components.emplace_back(std::make_unique<SpriteComponent>(this, compElement->Attribute("sprite")));
			}
			else if (currentComponent == "Body")
			{

				compElement->QueryFloatAttribute("density", &definition.physics.density);
				compElement->QueryFloatAttribute("restitution", &definition.physics.restitution);
				compElement->QueryFloatAttribute("angularDamping", &definition.physics.angularDamping);
				compElement->QueryFloatAttribute("linearDamping", &definition.physics.linearDamping);
				compElement->QueryBoolAttribute("physicsOn", &definition.physics.physicsOn);
				std::string bodyType{ compElement->Attribute("bodyType") };
				std::string bodyShape{ compElement->Attribute("bodyShape") };
				

				if (bodyType == "dynamic") { definition.physics.bodyType = BodyType::Dynamic; }
				else if (bodyType == "staticBody") { definition.physics.bodyType = BodyType::Static; }

				if (bodyShape == "rectangle") { definition.physics.bodyShape = BodyShape::Rectangle; }
				else if (bodyShape == "circle") { definition.physics.bodyShape = BodyShape::Circle; }


				definition.components.emplace_back(std::make_unique<BodyComponent>());
			}
			else if (currentComponent == "UserInput") definition.components.emplace_back(std::make_unique<UserInputComponent>());
			else if (currentComponent == "Backpack") definition.components.emplace_back(std::make_unique<BackpackComponent>());
			else if (currentComponent == "Inventory") definition.components.emplace_back(std::make_unique<InventoryComponent>());
			else if (currentComponent == "Ghost") definition.components.emplace_back(std::make_unique<GhostComponent>());
			else if (currentComponent == "Health") definition.components.emplace_back(std::make_unique<HealthComponent>());
			// if we have a misspeleed or non-existant component name in the file
			else
			{
				std::cout << "INVALID component in assets.xml: " << currentComponent << std::endl;
				return;
			}

			compElement = compElement->NextSiblingElement();
		}

		//Each asset should have at least one component!
		if (definition.components.empty()) return;
		oLibrary->addAsset(aName, std::move(definition));

		//Get the next Asset
		asset = asset->NextSiblingElement();
	} while (asset);
}



