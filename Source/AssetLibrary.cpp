#include <map>

#include "AssetLibrary.h"
#include "Texture.h"
#include "GraphicsDevice.h"
#include "ComponentsList.h"

AssetLibrary::~AssetLibrary()
{

}

bool AssetLibrary::initialize(SoundDevice * sDevice, GraphicsDevice * gDevice)
{
	if (sDevice == nullptr || gDevice == nullptr)
	{
		printf("Bad sound or graphic's device");
		return false;
	}
	else
	{
		this->sDevice = sDevice;
		this->gDevice = gDevice;
		return true;
	}

}

Texture * AssetLibrary::getArtAsset(std::string searchString)
{
	return artLibrary.find(searchString)->second.get();
}

bool AssetLibrary::setArtAsset(std::string name, std::string path)
{
	artLibrary[name] = std::make_unique<Texture>();
	if (!artLibrary[name]->load(gDevice->GetRenderer(), path)) 
	{ 
		std::map<std::string, std::unique_ptr<Texture>>::iterator artIter = artLibrary.find(name);
		artLibrary.erase(artIter);
		return false; 
	}
	else
	{
		return true;
	}
}
//**************************************
//Based on your position in the game space and direction you are facing
//this method searches the library for the proper notice to display
//and returns that notice.
//if not found, it sets the text to nothing of the notice passed in and
//returns that notice.
AssetLibrary::GAME_NOTICE AssetLibrary::getNotice(GAME_NOTICE square)
//**************************************
{
	for (auto notice : noticeLibrary)
	{
		if (notice.position == square.position && notice.direction == square.direction)
		{
			return notice;
		}
	}
	square.text = "";
	return square;
}

bool AssetLibrary::setNotice(GAME_NOTICE notice)
{
	if (notice.text == "")
	{
		return false;
	}
	else
	{
		noticeLibrary.push_back(notice);
	}
}

bool AssetLibrary::removeNotice(GAME_NOTICE notice)
{
	std::vector<GAME_NOTICE>::iterator noticeIter;
	//iterate through all notices
	for (noticeIter = noticeLibrary.begin(); noticeIter != noticeLibrary.end(); noticeIter++)
	{
		//if position and direction match, delete the notice from the library.
		if (noticeIter->position == notice.position && noticeIter->direction == notice.direction)
		{
			noticeLibrary.erase(noticeIter);
			noticeIter--;
			return true;
		}
	}
	//if we did not find a notice, we return false.
	return false;
}

ObjectFactory::GAME_OBJECT_STATS AssetLibrary::getObjectStats(std::string name)
{
	return (objectCreationLibrary.find(name)->second);
}

bool AssetLibrary::setObjectStats(std::string name, ObjectFactory::GAME_OBJECT_STATS stats)
{
	if (name == "")	return false;
	else
	{
		objectCreationLibrary[name] = stats;
		return true;
	}
}

PhysicsDevice::GAME_PHYSICS AssetLibrary::getObjectPhysics(std::string name)
{
	return(physicsLibrary.find(name)->second);
}

bool AssetLibrary::setObjectPhysics(std::string name, PhysicsDevice::GAME_PHYSICS physics)
{
	//this game never has friction.
	if (name == "") return false;
	else
	{
		physics.friction = 0.0f;
		physicsLibrary[name] = physics;
		return true;
	}
}

std::vector<Component*> AssetLibrary::getComponents(std::string name, GameObject * owner)
{
	//Vector of pointers to return
	//will be made into unique_ptrs in GameObject class
	std::vector<Component*> componentListPtrs;

	//finds the list of components associated with the name of the object passed.
	std::vector<GAME_COMPONENT_LIST> componentList = componentLibrary.find(name)->second;

	//Iterate through the list of components
	//add the proper component to the list of components to return.
	for (auto comp : componentList)
	{
		switch (comp)
		{
		case GAME_BODY_COMP:
			componentListPtrs.push_back(new BodyComponent(owner));
			break;
		case GAME_HEALTH_COMP:
			componentListPtrs.push_back(new HealthComponent(owner));
			break;
		case GAME_RENDERER_COMP:
			componentListPtrs.push_back(new RendererComponent(owner));
			break;
		case GAME_USERINPUT_COMP:
			componentListPtrs.push_back(new UserInputComponent(owner));
			break;
		case GAME_BACKPACK_COMP:
			componentListPtrs.push_back(new BackpackComponent(owner));
			break;
		case GAME_INVENTORY_COMP:
			componentListPtrs.push_back(new InventoryComponent(owner));
			break;
		case GAME_GHOST_COMP:
			componentListPtrs.push_back(new GhostComponent(owner));
			break;
		default:
			break;
		}
	}
	//return the completed list
	return componentListPtrs;
}

bool AssetLibrary::setComponentList(std::string name, std::vector<GAME_COMPONENT_LIST> componentList)
{
	if (name == "") return false;
	else
	{
		componentLibrary[name] = componentList;
		return true;
	}
}

Mix_Chunk * AssetLibrary::getSoundEffect(std::string name)
{
	std::map<std::string, Mix_Chunk*>::iterator soundIter = soundEffectLibrary.find(name);
	if (soundIter == soundEffectLibrary.end())
	{
		{printf("Sound Effect File not found!"); }
		return nullptr;
	}
	return soundIter->second;
}

Mix_Music * AssetLibrary::getMusic(std::string name)
{
	std::map<std::string,Mix_Music*>::iterator musicIter = musicLibrary.find(name);
	//make sure we found one.
	if (musicIter == musicLibrary.end())
	{
		{printf("Background File not found!"); }
		return nullptr;
	}
	else return musicIter->second;;
}

bool AssetLibrary::setSoundEffect(std::string name, std::string path)
{
	if (soundEffectLibrary[name] = Mix_LoadWAV(path.c_str())) return true;
	return false;
}

bool AssetLibrary::setBackgroundMusic(std::string name, std::string path)
{
	//Mix_Load
	if (musicLibrary[name] = Mix_LoadMUS(path.c_str())) return true;
	return false;
}
//**************************************
//Have not set this up yet, as there has bee no need for it.
bool AssetLibrary::removeSound(std::string name)
//**************************************
{
	return false;
}
