
#include "ResourceManager.h"
#include "DeviceList.h"
#include "LibraryList.h"
#include "ObjectFactory.h"
#include "Definitions.h"
#include "tinyxml\tinyxml.h"
#include "Box2DDebugDraw.h"



ResourceManager::ResourceManager(){}
ResourceManager::~ResourceManager(){}



//**************************************
//prepares all asset libraries based on path passed xml file
//and creastes and initialzies all devices
bool ResourceManager::Initialize(GAME_INT SCREEN_WIDTH, GAME_INT SCREEN_HEIGHT, std::string assetPath)
{
	//don't load the basement
	loadBasement = false;

	GAME_FPS = 100;
	//========================================
	//Construct Object Factory
	//========================================
	factory = std::make_shared<ObjectFactory>();


	//========================================
	//Construct Input Device
	//========================================
	iDevice = std::make_shared<InputDevice>();
	if(!iDevice->Initialize())
	{
		printf( "Input Device could not initialize!");
		exit(1);
	}
		//========================================
	//Construct Graphical Device
	//========================================
	gDevice = std::make_shared<GraphicsDevice>(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!gDevice->Initialize(true))
	{
		printf( "Graphics Device could not Initialize!");
		exit(1);
	}
	//color of fonts
	GAME_RGBA RGBA;
	RGBA.R = 0;
	RGBA.G = 0;
	RGBA.B = 0;
	RGBA.A = 255;
	//set graphic device font
	gDevice -> SetFont("./Assets/Fonts/impact.ttf", 16, RGBA);
	//========================================
	//Construct Physics Device
	//========================================
	pDevice = std::make_shared<PhysicsDevice>(0,0);

	
	if(!pDevice -> Initialize())
	{
		printf("Physics Device could not intialize!");
		exit(1);
	}
	//========================================
	//Construct Sound Device
	//========================================
	sDevice = std::make_shared<SoundDevice>();
	
	if(!sDevice -> Initialize())
	{
		printf("Physics Device could not intialize!");
		exit(1);
	}

	//========================================
	//Construct Art Library
	//========================================
	aLibrary = std::make_shared<ArtAssetLibrary>();	
	//needs to be an xml file just likes physics.
	if(!aLibrary -> Initialize(gDevice.get())){return false;}

	//========================================
	//Construct Physics Library
	//========================================
	pLibrary = std::make_shared<PhysicsAssetLibrary>();
	if(!pLibrary -> Initialize()){return false;}

	//========================================
	//Construct Physics Library
	//========================================
	cLibrary = std::make_shared<ComponentAssetLibrary>();
	if(!cLibrary -> Initialize()){return false;}

	//========================================
	//Construct Objects Library
	//========================================
	oLibrary = std::make_shared<ObjectAssetLibrary>();
	if(!oLibrary -> Initialize()){return false;}

	//========================================
	//Construct Notices Library
	//========================================
	nLibrary = std::make_shared<NoticesAssetLibrary>();
	if(!nLibrary -> Initialize()){return false;}

	//========================================
	//Construct Sound Library
	//========================================
	sLibrary = std::make_shared<SoundAssetLibrary>();
	if(!sLibrary -> Initialize(sDevice.get())){return false;}

	//========================================
	//Populate Libraries
	//========================================
	
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
		std::vector<ComponentAssetLibrary::GAME_COMPONENT_LIST> componentList;
		
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
				aLibrary -> AddAsset(aName, compElement -> Attribute("sprite"));
				//add the component to the list
				componentList.push_back(ComponentAssetLibrary::GAME_RENDERER_COMP);
			}
			else if(currentComponent == "Body")
			{
				PhysicsDevice::GAME_PHYSICS physics;
				//Get physics properties
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
				pLibrary -> AddAsset(aName, physics);
				
				//add component to list
				componentList.push_back(ComponentAssetLibrary::GAME_BODY_COMP);
			}
			else if(currentComponent == "Health")
			{
				// Get the health
				ObjectFactory::GAME_OBJECT_STATS stats;
				compElement -> QueryIntAttribute("health", &stats.health);
				//add to library
				oLibrary -> AddAsset(aName, stats);
				//add component
				componentList.push_back(ComponentAssetLibrary::GAME_HEALTH_COMP);
			}
			else if(currentComponent == "UserInput") componentList.push_back(ComponentAssetLibrary::GAME_USERINPUT_COMP);
			else if(currentComponent == "Backpack") componentList.push_back(ComponentAssetLibrary::GAME_BACKPACK_COMP);
			else if(currentComponent == "Inventory") componentList.push_back(ComponentAssetLibrary::GAME_INVENTORY_COMP);
			else if(currentComponent == "Ghost") componentList.push_back(ComponentAssetLibrary::GAME_GHOST_COMP);
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
		cLibrary -> AddAsset(aName, componentList);
		
		//Get the next Asset
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
		NoticesAssetLibrary::GAME_NOTICE notice;
		int x,y;
		std::string direction;
		notices -> QueryIntAttribute("x", &x);
		notices -> QueryIntAttribute("y", &y);
		notices -> QueryStringAttribute("direction", &direction);
		notice.text = notices ->GetText();
		
		//store in notice, in proper format.
		notice.x = x;
		notice.y = y;
		if(direction == "N") notice.direction = N;
		else if(direction == "E") notice.direction = E;
		else if(direction == "S") notice.direction = S;
		else if(direction == "W") notice.direction = W;
		
		//add it to the library.
		nLibrary -> AddAsset(notice);
		
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
			sLibrary -> AddBackgroundMusic(name, path);
		}
		else
		{
			sLibrary -> AddSoundEffect(name, path);
		}
		sounds = sounds -> NextSiblingElement();
	}



	//***********************************************************
	
	//
	//Set-up debugging
	//
	Box2DDebugDraw* debugDraw = new Box2DDebugDraw();
	debugDraw->Initialize(this);
    debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit);  //Turn on shape (red color) and aabb (green) 

	//Add the Debug Draw to the world
	if(debugDraw!=NULL)
	{
		pDevice -> getWorld() -> SetDebugDraw(debugDraw);
	}
	return true;
}

//**************************************
//Deletes all the devices.
//need to switch to smart pointers.
bool ResourceManager::Shutdown()
//**************************************

{

	iDevice = nullptr;

	sLibrary->Finish();
	sLibrary = NULL;

	gDevice->ShutDown();
	gDevice = NULL;

	sDevice->Shutdown();
	sDevice = NULL;

	pDevice = NULL;


	aLibrary = NULL;

	pLibrary = NULL;

	oLibrary = NULL;

	nLibrary = NULL;

	factory = NULL;



	return true;
}