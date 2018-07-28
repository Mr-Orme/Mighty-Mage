
#include "ResourceManager.h"
#include "DeviceList.h"
#include "AssetLibrary.h"
#include "ObjectFactory.h"
#include "Definitions.h"
#include "tinyxml\tinyxml.h"
//#include "Box2DDebugDraw.h"



ResourceManager::ResourceManager(){}
ResourceManager::~ResourceManager(){}



//**************************************
//prepares all asset libraries based on path passed xml file
//and creastes and initialzies all devices
bool ResourceManager::Initialize(GAME_INT SCREEN_WIDTH, GAME_INT SCREEN_HEIGHT, std::string assetPath)
{

	/*TODO: Create Level class and have vector of levels. . . . Level needs:
		1. Physics World - make swappable worlds in physics device
		2. Objects vector
	*/
	//don't load the basement
	loadBasement = false;

	//TODO: XML of game values including:
	//1. FPS
	//2. Screen dimensions
	GAME_FPS = 100;
	//========================================
	//Construct Object Factory
	//========================================
	factory = std::make_unique<ObjectFactory>();


	//========================================
	//Construct Input Device
	//========================================
	iDevice = std::make_unique<InputDevice>();
	if(!iDevice->Initialize())
	{
		printf( "Input Device could not initialize!");
		exit(1);
	}
		//========================================
	//Construct Graphical Device
	//========================================
	gDevice = std::make_unique<GraphicsDevice>(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!gDevice->Initialize(true))
	{
		printf( "Graphics Device could not Initialize!");
		exit(1);
	}
	//color of fonts
	GAME_RGBA RGBA{ 0,0,0,255 };
	
	//set graphic device font
	gDevice -> SetFont("./Assets/Fonts/impact.ttf", 16, RGBA);

	//========================================
	//Construct Physics Device
	//========================================
	pDevice = std::make_unique<PhysicsDevice>(0,0);

	
	if(!pDevice -> Initialize())
	{
		printf("Physics Device could not intialize!");
		exit(1);
	}

	//========================================
	//Construct Sound Device
	//========================================
	sDevice = std::make_unique<SoundDevice>();

	if (!sDevice->Initialize(this))
	{
		printf("Physics Device could not intialize!");
		exit(1);
	}

	//========================================
	//Construct Asset Library
	//========================================
	assetLibrary = std::make_unique<AssetLibrary>();
	//needs to be an xml file just likes physics.
	if (!assetLibrary->initialize(sDevice.get(), gDevice.get())) { exit(1); }
	


	
	//prepare XML file for parsing.
	TiXmlDocument assetFile(assetPath);
	if(!assetFile.LoadFile()){return false;};
	TiXmlElement* assetType = assetFile.FirstChildElement();
	
	//get first asset
	TiXmlElement* asset = assetType -> FirstChildElement();
	//add each asset into each library.
	do
	{
		//get the name of the asset
		std::string aName = asset -> Attribute("name");
	
		//where we will store the components.
		std::vector<AssetLibrary::GAME_COMPONENT_LIST> componentList;
		
		//move to the components of the xml
		TiXmlElement* compElement = asset -> FirstChildElement();
		
		//Add each component to the vector
		while (compElement)
		{
			//get the component's name
			std::string currentComponent = compElement -> Attribute("name");
			//find the component we just grabbed
			if(currentComponent == "Renderer")
			{
				//add the coresponding asset to the library.
				assetLibrary -> setArtAsset(aName, compElement -> Attribute("sprite"));
				//add the component to the list
				componentList.push_back(AssetLibrary::GAME_RENDERER_COMP);
			}
			else if(currentComponent == "Body")
			{
				//TODO: Consolidate all initial values into one struct
				PhysicsDevice::GAME_PHYSICS physics;
				//get physics properties
				compElement -> QueryFloatAttribute("density", &physics.density);
				compElement -> QueryFloatAttribute("restitution", &physics.restitution);
				compElement -> QueryFloatAttribute("angularDamping", &physics.angularDamping);
				compElement -> QueryFloatAttribute("linearDamping", &physics.linearDamping);
				compElement -> QueryBoolAttribute("physicsOn", &physics.physicsOn);
				std::string bodyType;
				std::string bodyShape;
				compElement -> QueryStringAttribute("bodyType", &bodyType);
				compElement -> QueryStringAttribute("bodyShape", &bodyShape);
		
				//convert strings to enums
				if(bodyType == "GAME_DYNAMIC"){physics.bodyType = PhysicsDevice::GAME_DYNAMIC;}
				else if (bodyType == "GAME_STATIC"){physics.bodyType = PhysicsDevice::GAME_STATIC;}
		
				if(bodyShape == "GAME_RECTANGLE"){physics.objectShape = PhysicsDevice::GAME_RECTANGLE;}
				else if (bodyShape == "GAME_CIRCLE"){physics.objectShape = PhysicsDevice::GAME_CIRCLE;}
				//add to library
				assetLibrary -> setObjectPhysics(aName, physics);
				
				//add component to list
				componentList.push_back(AssetLibrary::GAME_BODY_COMP);
			}
			else if(currentComponent == "Health")
			{
				// get the health
				ObjectFactory::GAME_OBJECT_STATS stats;
				compElement -> QueryIntAttribute("health", &stats.health);
				//add to library
				assetLibrary -> setObjectStats(aName, stats);
				//add component
				componentList.push_back(AssetLibrary::GAME_HEALTH_COMP);
			}
			else if (currentComponent == "UserInput")
			{
				componentList.push_back(AssetLibrary::GAME_USERINPUT_COMP);
			}
			else if(currentComponent == "Backpack") componentList.push_back(AssetLibrary::GAME_BACKPACK_COMP);
			else if(currentComponent == "Inventory") componentList.push_back(AssetLibrary::GAME_INVENTORY_COMP);
			else if(currentComponent == "Ghost") componentList.push_back(AssetLibrary::GAME_GHOST_COMP);
			// if we have a misspeleed or non-existant component name in the file
			else
			{
				std::cout << "INVALID component in assets.xml: " << currentComponent << std::endl;
				return false;
			}

			compElement = compElement -> NextSiblingElement();
		}
		
		//Each asset should have at least one component!
		if(componentList.empty()) return false;
		
		//add to library
		assetLibrary -> setComponentList(aName, componentList);
		
		//get the next Asset
		asset = asset -> NextSiblingElement();
	} while (asset);
	
	//*********************Load Notices***************************
	//move to notice section of file
	assetType = assetType -> NextSiblingElement();
	//grab first notice
	TiXmlElement* notices = assetType -> FirstChildElement();
	do
	{
		//get information from file
		AssetLibrary::GAME_NOTICE notice;
		int x,y;
		std::string direction;
		notices -> QueryIntAttribute("x", &x);
		notices -> QueryIntAttribute("y", &y);
		notices -> QueryStringAttribute("direction", &direction);
		notice.text = notices ->GetText();
		
		//store in notice, in proper format.
		notice.position = { (GAME_FLT)x,(GAME_FLT)y };
		if(direction == "N") notice.direction = N;
		else if(direction == "E") notice.direction = E;
		else if(direction == "S") notice.direction = S;
		else if(direction == "W") notice.direction = W;
		
		//add it to the library.
		assetLibrary -> setNotice(notice);
		
		//get the next notice
		notices = notices -> NextSiblingElement();
	} while (notices);
	//***********************************************************

	//*********************Load Sounds***************************
	//move tos ound section of file
	assetType = assetType -> NextSiblingElement();
	//grab first sound
	TiXmlElement* sounds = assetType -> FirstChildElement();
	while (sounds)
	{
		//get information from file
		std::string name;
		sounds -> QueryStringAttribute("name", &name);
		std::string path;
		sounds -> QueryStringAttribute("path", &path);
		bool background;
		sounds -> QueryBoolAttribute("background", &background);

		//add to library based on whether it is background music or not.
		if(background)
		{
			assetLibrary -> setBackgroundMusic(name, path);
		}
		else
		{
			assetLibrary -> setSoundEffect(name, path);
		}
		sounds = sounds -> NextSiblingElement();
	}



	//***********************************************************
	
	//
	//set-up debugging
	//
	//Box2DDebugDraw* debugDraw = new Box2DDebugDraw();
	//debugDraw->Initialize(this);
 //   debugDraw->setFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit);  //Turn on shape (red color) and aabb (green) 

	////Add the Debug Draw to the world
	//if(debugDraw!=NULL)
	//{
	//	pDevice -> getWorld() -> setDebugDraw(debugDraw);
	//}
	return true;
}

//**************************************
//Deletes all the devices.
//need to switch to smart pointers.
bool ResourceManager::Shutdown()
//**************************************

{

	iDevice = nullptr;

	gDevice->ShutDown();
	gDevice = nullptr;

	sDevice->Shutdown();
	sDevice = nullptr;

	pDevice = nullptr;


	assetLibrary = nullptr;

	factory = nullptr;



	return true;
}

GraphicsDevice * ResourceManager::getGraphicsDevice()
{
	return gDevice.get();
}

InputDevice * ResourceManager::getInputDevice()
{
	return iDevice.get();
}

PhysicsDevice * ResourceManager::getPhysicsDevice()
{
	return pDevice.get();
}

SoundDevice * ResourceManager::getSoundDevice()
{
	return sDevice.get();
}

AssetLibrary * ResourceManager::getAssetLibrary()
{
	return assetLibrary.get();
}

ObjectFactory * ResourceManager::getObjectFactory()
{
	return factory.get();
}

void ResourceManager::setGraphicsDevice(GraphicsDevice * gDevice)
{
	this->gDevice = std::unique_ptr<GraphicsDevice>(gDevice);
}

void ResourceManager::setInputDevice(InputDevice * iDevice)
{
	this->iDevice = std::unique_ptr<InputDevice>(iDevice);
}

void ResourceManager::setPhysicsDevice(PhysicsDevice * pDevice)
{
	this->pDevice = std::unique_ptr<PhysicsDevice>(pDevice);
}

void ResourceManager::setSoundDevice(SoundDevice * sDevice)
{
	this->sDevice = std::unique_ptr<SoundDevice>(sDevice);
}

void ResourceManager::setAssetLibrary(AssetLibrary * assetLibrary)
{
	this->assetLibrary = std::unique_ptr<AssetLibrary>(assetLibrary);
}
