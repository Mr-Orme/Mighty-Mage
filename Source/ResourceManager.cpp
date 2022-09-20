
#include "ResourceManager.h"
#include "ObjectFactory.h"
#include "Definitions.h"
#include "tinyxml\tinyxml.h"
#include "Box2DDebugDraw.h"


ResourceManager::ResourceManager(){}
ResourceManager::~ResourceManager(){}



//**************************************
//prepares all asset libraries based on path passed xml file
//and creastes and initialzies all devices
bool ResourceManager::initialize(int SCREEN_WIDTH, int SCREEN_HEIGHT, std::string assetPath)
{
	//don't load the basement
	loadBasement = false;

	GAME_FPS = 100;
	//========================================
	//Construct Object Factory
	//========================================
	factory = std::make_unique<ObjectFactory>();


	//========================================
	//Construct Input Device
	//========================================
	iDevice = std::make_unique<InputDevice>();
	if(!iDevice->initialize())
	{
		printf( "Input Device could not initialize!");
		exit(1);
	}
		//========================================
	//Construct Graphical Device
	//========================================
	gDevice = std::make_unique<GraphicsDevice>(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!gDevice->initialize(true))
	{
		printf( "Graphics Device could not Initialize!");
		exit(1);
	}
	//color of fonts
	RGBA RGBA{ 0, 0, 0, 255 };
	
	//set graphic device font
	gDevice -> SetFont("./Assets/Fonts/impact.ttf", 16, RGBA);
	//========================================
	//Construct Physics Device
	//========================================
	pDevice = std::make_unique<PhysicsDevice>(0,0);

	
	if(!pDevice -> initialize())
	{
		printf("Physics Device could not intialize!");
		exit(1);
	}
	//========================================
	//Construct Sound Device
	//========================================
	sDevice = std::make_unique<SoundDevice>();
	
	if(!sDevice -> initialize())
	{
		printf("Physics Device could not intialize!");
		exit(1);
	}

	//========================================
	//Construct Art Library
	//========================================
	aLibrary = std::make_unique<ArtAssetLibrary>();	
	//needs to be an xml file just likes physics.
	if(!aLibrary -> initialize(gDevice.get())){return false;}

	//========================================
	//Construct Physics Library
	//========================================
	pLibrary = std::make_unique<PhysicsAssetLibrary>();
	if(!pLibrary -> initialize()){return false;}

	//========================================
	//Construct Physics Library
	//========================================
	cLibrary = std::make_unique<ComponentAssetLibrary>();
	if(!cLibrary -> initialize()){return false;}

	//========================================
	//Construct Objects Library
	//========================================
	oLibrary = std::make_unique<ObjectAssetLibrary>();
	if(!oLibrary -> initialize()){return false;}

	//========================================
	//Construct Notices Library
	//========================================
	nLibrary = std::make_unique<NoticesAssetLibrary>();
	if(!nLibrary -> initialize()){return false;}

	//========================================
	//Construct Sound Library
	//========================================
	sLibrary = std::make_unique<SoundAssetLibrary>();
	if(!sLibrary -> initialize(sDevice.get())){return false;}

	//========================================
	//Populate Libraries
	//========================================
	
	//prepare XML file for parsing.
	TiXmlDocument assetFile(assetPath);
	if(!assetFile.LoadFile()){return false;};
	TiXmlElement* assetType = assetFile.FirstChildElement()->FirstChildElement();
	
	//get first asset
	TiXmlElement* asset = assetType -> FirstChildElement();
	//add each asset into each library.
	do
	{
		//get the name of the asset
		std::string aName = asset -> Attribute("name");
	
		//where we will store the components.
		std::vector<Components> componentList;
		
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
				componentList.push_back(Components::renderer);
			}
			else if(currentComponent == "Body")
			{
				PhysicsStats physics;
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
				if(bodyType == "dynamic"){physics.bodyType = PhysicsStats::BodyType::dynamic;}
				else if (bodyType == "staticBody"){physics.bodyType = PhysicsStats::BodyType::staticBody;}
		
				if(bodyShape == "rectangle"){physics.objectShape = PhysicsStats::BodyShape::rectangle;}
				else if (bodyShape == "circle"){physics.objectShape = PhysicsStats::BodyShape::circle;}
				//add to library
				pLibrary -> AddAsset(aName, physics);
				
				//add component to list
				componentList.push_back(Components::body);
			}
			else if(currentComponent == "Health")
			{
				// Get the health
				ObjectStats stats;
				compElement -> QueryIntAttribute("health", &stats.health);
				//add to library
				oLibrary -> AddAsset(aName, stats);
				//add component
				componentList.push_back(Components::health);
			}
			else if(currentComponent == "UserInput") componentList.push_back(Components::userInput);
			else if(currentComponent == "Backpack") componentList.push_back(Components::backpack);
			else if(currentComponent == "Inventory") componentList.push_back(Components::inventory);
			else if(currentComponent == "Ghost") componentList.push_back(Components::ghost);
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
		Notice notice;
		int x,y;
		std::string direction;
		notices -> QueryIntAttribute("x", &x);
		notices -> QueryIntAttribute("y", &y);
		notices -> QueryStringAttribute("direction", &direction);
		notice.text = notices ->GetText();
		
		//store in notice, in proper format.
		notice.x = x;
		notice.y = y;
		if(direction == "N") notice.direction = Direction::N;
		else if(direction == "E") notice.direction = Direction::E;
		else if(direction == "S") notice.direction = Direction::S;
		else if(direction == "W") notice.direction = Direction::W;
		
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
	debugDraw->initialize(this);
    debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit);  //Turn on shape (red color) and aabb (green) 

	//Add the Debug Draw to the world
	if(debugDraw!=nullptr)
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
	sLibrary = nullptr;

	gDevice->ShutDown();
	gDevice = nullptr;

	sDevice->Shutdown();
	sDevice = nullptr;

	pDevice = nullptr;


	aLibrary = nullptr;

	pLibrary = nullptr;

	oLibrary = nullptr;

	nLibrary = nullptr;

	factory = nullptr;



	return true;
}