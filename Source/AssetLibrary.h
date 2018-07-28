#ifndef ASSETLIBRARY_H
#define ASSETLIBRARY_H

#include <map>
#include <string>
#include <vector>

#include "Definitions.h"
#include "ObjectFactory.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"

class Texture;
class GraphicsDevice;
class GameObject;
class Component;

class AssetLibrary
{
public:
	AssetLibrary() {}
	~AssetLibrary();
	
	bool initialize(SoundDevice* sDevice, GraphicsDevice* gDevice);

	Texture* getArtAsset(std::string searchString);
	bool setArtAsset(std::string name, std::string path);

	struct GAME_NOTICE
	{
		GAME_VEC position;
		GAME_DIRECTION direction;
		std::string text;
	};
	GAME_NOTICE getNotice(GAME_NOTICE square);
	bool setNotice(GAME_NOTICE notice);
	bool removeNotice(GAME_NOTICE notice);

	ObjectFactory::GAME_OBJECT_STATS getObjectStats(std::string name);
	bool setObjectStats(std::string name, ObjectFactory::GAME_OBJECT_STATS stats);

	PhysicsDevice::GAME_PHYSICS getObjectPhysics(std::string name);
	bool setObjectPhysics(std::string name, PhysicsDevice::GAME_PHYSICS physics);

	enum GAME_COMPONENT_LIST
	{
		GAME_BODY_COMP,
		GAME_HEALTH_COMP,
		GAME_RENDERER_COMP,
		GAME_USERINPUT_COMP,
		GAME_BACKPACK_COMP,
		GAME_INVENTORY_COMP,
		GAME_GHOST_COMP,
	};

	std::vector<Component*> getComponents(std::string name, GameObject* owner);
	bool setComponentList(std::string name, std::vector<GAME_COMPONENT_LIST> componentList);

	Mix_Chunk* getSoundEffect(std::string name);
	Mix_Music* getMusic(std::string name);
	bool setSoundEffect(std::string name, std::string path);
	bool setBackgroundMusic(std::string name, std::string path);
	bool removeSound(std::string name);

	


private:
	std::map<std::string, Texture*> artLibrary;
	std::vector<GAME_NOTICE> noticeLibrary;
	std::map<std::string, ObjectFactory::GAME_OBJECT_STATS> objectCreationLibrary;
	std::map<std::string, PhysicsDevice::GAME_PHYSICS> physicsLibrary;
	std::map<std::string, std::vector<GAME_COMPONENT_LIST>> componentLibrary;
	std::map<std::string, Mix_Chunk* > soundEffectLibrary;
	std::map<std::string, Mix_Music* > musicLibrary;
	
	SoundDevice* sDevice;
	GraphicsDevice* gDevice;

};
#endif // !ASSETLIBRARIES_H
