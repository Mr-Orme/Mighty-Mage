
#include <iostream>
#include "ResourceManager.h"
#include "ObjectLibrary.h"
#include "ObjectFactory.h"
#include "ComponentsList.h"
#include "Box2DDebugDraw.h"
#include "Initializers.h"
#include "SoundDevice.h"
#include "NoticesAssetLibrary.h"
#include "SoundAssetLibrary.h"
#include "View.h"

ResourceManager::ResourceManager(Vector2D screenDimensions, std::string assetPath):
	factory(std::make_unique<ObjectFactory>()),
	iDevice(std::make_unique<InputDevice>()),
	oLibrary(std::make_unique<ObjectLibrary>()),
	nLibrary(std::make_unique<NoticesAssetLibrary>())
{
	gDevice = std::make_unique<GraphicsDevice>(screenDimensions, "./Assets/Fonts/impact.ttf", 16, RGBA{ 0, 0, 0, 255 });
	pDevice = std::make_unique<PhysicsDevice>(Vector2D{ 0,0 }, this);

	sLibrary = std::make_unique<SoundAssetLibrary>();
	sDevice = std::make_unique<SoundDevice>(sLibrary.get());

	loadLibraries(assetPath);
}

ResourceManager::~ResourceManager(){
}

GraphicsDevice* ResourceManager::getGraphicsDevice()
{
	return gDevice.get();
}

InputDevice* ResourceManager::getInputDevice()
{
	return iDevice.get();
}

PhysicsDevice* ResourceManager::getPhysicsDevice()
{
	return pDevice.get();
}

SoundDevice* ResourceManager::getSoundDevice()
{
	return sDevice.get();
}

ObjectLibrary* ResourceManager::getObjectLibrary()
{
	return oLibrary.get();
}

NoticesAssetLibrary* ResourceManager::getNoticesLibrary()
{
	return nLibrary.get();
}

SoundAssetLibrary* ResourceManager::getSoundLibrary()
{
	return sLibrary.get();
}

ObjectFactory* ResourceManager::getObjectFactory()
{
	return factory.get();
}

std::tuple<Vector2D, Direction> ResourceManager::getPlayerStart() const
{
	return { playerStart, playerDirection };
}

void ResourceManager::changeLevel(Levels exit, std::optional<Vector2D> playerStart, std::optional<Direction> direction)
{
	this->toLoad = exit;
	if (playerStart) this->playerStart = *playerStart;
	if (direction) this->playerDirection = *direction;
}

Vector2D ResourceManager::pixel2Square(Vector2D position) const
{
	return
	{
		position.x / pixelsPerSquare,
		blocksPerMap - (position.y / pixelsPerSquare)
	};
}

Vector2D ResourceManager::square2Pixel(float x, float y) const
{
	float xInt{};
	float yInt{};
	float xfract{ modff(x, &xInt) };
	float yfract{ modff(y, &yInt) };
	
	return square2Pixel
	(
		{ (int)xInt, (int)yInt }, { (int)(xfract * 100.0f), (int)(yfract * 100.0f) }
	);
	
}

Vector2D ResourceManager::square2Pixel(Vector2D square, Vector2D fractional) const
{
	fractional = 
	{ 
		(int)((fractional.x / 100.0f) * pixelsPerSquare), 
		(int)((fractional.y / 100.0f) * pixelsPerSquare) 
	};
	return
	{
		square.x * pixelsPerSquare + fractional.x,
		(blocksPerMap - square.y) * pixelsPerSquare + fractional.y
	};
}

void ResourceManager::loadLibraries(std::string assetPath)
{	
	tinyxml2::XMLDocument assetFile;
	if (!assetFile.LoadFile(assetPath.c_str()) == tinyxml2::XML_SUCCESS) { return; };
	tinyxml2::XMLElement* assetType{ assetFile.FirstChildElement()->FirstChildElement() };

	loadObjects(assetType->FirstChildElement());

	assetType = assetType->NextSiblingElement();
	
	loadNotices(assetType->FirstChildElement());
	
	assetType = assetType->NextSiblingElement();
		
	loadSounds(assetType->FirstChildElement());
}

void ResourceManager::loadNotices(tinyxml2::XMLElement* notices)
{
	while (notices)
	{
		//get information from file TODO:: x and y directly in notice
		Notice notice;

		notices->QueryIntAttribute("x", &notice.square.x);
		notices->QueryIntAttribute("y", &notice.square.y);
		
		int direction{};
		notices->QueryIntAttribute("direction", &direction);
		notice.direction = (Direction)direction;
		
		notice.text = notices->GetText();

		nLibrary->addAsset(notice);

		notices = notices->NextSiblingElement();
	}
}

void ResourceManager::loadObjects(tinyxml2::XMLElement* asset)
{
	while (asset)
	{
		std::string objectName{ asset->Attribute("name") };

		oLibrary->addAsset(objectName, loadComponent(asset->FirstChildElement()));

		asset = asset->NextSiblingElement();
	}
}

void ResourceManager::loadSounds(tinyxml2::XMLElement* sounds)
{
	while (sounds)
	{
				
		int sound{ };
		sounds->QueryIntAttribute("name", &sound);
		std::string path{ sounds->Attribute("path") };
		
		bool background;
		sounds->QueryBoolAttribute("background", &background);

		if (background)
		{
			sLibrary->addBackgroundMusic((Levels)sound, path);
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
}

ObjectDefinition ResourceManager::loadComponent(tinyxml2::XMLElement* component)
{
	ObjectDefinition definition;
	while (component)
	{
		std::string currentComponent = component->Attribute("name");
		//find the component we just grabbed
		if (currentComponent == "Renderer")
		{
			definition.components.emplace_back(std::make_unique<SpriteComponent>(this, component->Attribute("sprite")));
		}
		else if (currentComponent == "Body")
		{

			component->QueryFloatAttribute("density", &definition.physics.density);
			component->QueryFloatAttribute("restitution", &definition.physics.restitution);
			component->QueryFloatAttribute("angularDamping", &definition.physics.angularDamping);
			component->QueryFloatAttribute("linearDamping", &definition.physics.linearDamping);
			component->QueryBoolAttribute("physicsOn", &definition.physics.physicsOn);
			std::string bodyType{ component->Attribute("bodyType") };
			std::string bodyShape{ component->Attribute("bodyShape") };


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
		else if (currentComponent == "Trigger")
			definition.components.emplace_back(std::make_unique<TriggerComponent>());
		else if (currentComponent == "Quest") definition.components.emplace_back(std::make_unique<QuestComponent>());
		else if (currentComponent == "Criteria") definition.components.emplace_back(std::make_unique<QuestCriteria>());
		// if we have a misspeleed or non-existant component name in the file
		else
		{
			std::cout << "INVALID component in assets.xml: " << currentComponent << std::endl;
			return definition;
		}

		component = component->NextSiblingElement();
	}
	return definition;
}



