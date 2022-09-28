
#include "ResourceManager.h"
#include "ComponentAssetLibrary.h"
#include "ObjectFactory.h"
#include "Definitions.h"

#include "Box2DDebugDraw.h"
#include "Initializers.h"
#include "tinyxml2.h"

ResourceManager::ResourceManager(Vector2D screenDimensions, std::string assetPath):
	factory(std::make_unique<ObjectFactory>()),
	iDevice(std::make_unique<InputDevice>()),
	pLibrary(std::make_unique<PhysicsAssetLibrary>()),
	oLibrary(std::make_unique<ObjectAssetLibrary>()),
	nLibrary(std::make_unique<NoticesAssetLibrary>())
{
	//don't load the basement
	loadBasement = false;

	RGBA fontColor{ 0, 0, 0, 255 };
	gDevice = std::make_unique<GraphicsDevice>(screenDimensions, "./Assets/Fonts/impact.ttf", 16, fontColor);
	pDevice = std::make_unique<PhysicsDevice>(Vector2D{ 0,0 }, this);

	sLibrary = std::make_unique<SoundAssetLibrary>();
	sDevice = std::make_unique<SoundDevice>(sLibrary.get());
	
	aLibrary = std::make_unique<ArtAssetLibrary>(gDevice.get());
	cLibrary = std::make_unique<ComponentAssetLibrary>(this);

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
		//get the name of the asset
		std::string aName = asset->Attribute("name");

		//where we will store the components.
		std::vector<ComponentAssetLibrary::Components> componentList;

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
				//add the coresponding asset to the library.
				aLibrary->addAsset(aName, compElement->Attribute("sprite"));
				//add the component to the list
				componentList.push_back(ComponentAssetLibrary::Components::renderer);
			}
			else if (currentComponent == "Body")
			{
				PhysicsStats physics;
				//Get physics properties
				compElement->QueryFloatAttribute("density", &physics.density);
				compElement->QueryFloatAttribute("restitution", &physics.restitution);
				compElement->QueryFloatAttribute("angularDamping", &physics.angularDamping);
				compElement->QueryFloatAttribute("linearDamping", &physics.linearDamping);
				compElement->QueryBoolAttribute("physicsOn", &physics.physicsOn);
				std::string bodyType{ compElement->Attribute("bodyType") };
				std::string bodyShape{ compElement->Attribute("bodyShape") };
				
				//convert strings to enums
				if (bodyType == "dynamic") { physics.bodyType = BodyType::Dynamic; }
				else if (bodyType == "staticBody") { physics.bodyType = BodyType::Static; }

				if (bodyShape == "rectangle") { physics.bodyShape = BodyShape::Rectangle; }
				else if (bodyShape == "circle") { physics.bodyShape = BodyShape::Circle; }
				//add to library
				pLibrary->addAsset(aName, physics);

				//add component to list
				componentList.push_back(ComponentAssetLibrary::Components::body);
			}
			else if (currentComponent == "Health")
			{
				// Get the health
				ObjectStats stats;
				compElement->QueryIntAttribute("health", &stats.health);
				//add to library
				oLibrary->addAsset(aName, stats);
				//add component
				componentList.push_back(ComponentAssetLibrary::Components::health);
			}
			else if (currentComponent == "UserInput") componentList.push_back(ComponentAssetLibrary::Components::userInput);
			else if (currentComponent == "Backpack") componentList.push_back(ComponentAssetLibrary::Components::backpack);
			else if (currentComponent == "Inventory") componentList.push_back(ComponentAssetLibrary::Components::inventory);
			else if (currentComponent == "Ghost") componentList.push_back(ComponentAssetLibrary::Components::ghost);
			// if we have a misspeleed or non-existant component name in the file
			else
			{
				std::cout << "INVALID component in assets.xml: " << currentComponent << std::endl;
				return;
			}

			compElement = compElement->NextSiblingElement();
		}

		//Each asset should have at least one component!
		if (componentList.empty()) return;

		//add to library
		cLibrary->addAsset(aName, componentList);

		//Get the next Asset
		asset = asset->NextSiblingElement();
	} while (asset);
}



