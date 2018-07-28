#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Definitions.h"

class ObjectFactory;


class AssetLibrary;

class InputDevice;
class GraphicsDevice;
class PhysicsDevice;
class SoundDevice;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	
	bool Initialize(GAME_INT SCREEN_WIDTH, GAME_INT SCREEN_HEIGHT, std::string assetPath);
	bool Shutdown();

	//Device getters
	GraphicsDevice* getGraphicsDevice();
	InputDevice* getInputDevice();
	PhysicsDevice* getPhysicsDevice();
	SoundDevice* getSoundDevice();

	//Device setters
	AssetLibrary* getAssetLibrary();
	

	//ObjectFactor getter
	ObjectFactory* getObjectFactory();

	//Other getters
	GAME_INT getFPS(){return GAME_FPS;}
	GAME_VEC getCityCorner(){return cityCorner;}
	GAME_LEVEL getLevel(){return level;}
	bool getLoadBasement(){return loadBasement;}

	//Device setters
	void setGraphicsDevice(GraphicsDevice* gDevice);
	void setInputDevice(InputDevice* iDevice);
	void setPhysicsDevice(PhysicsDevice* pDevice);
	void setSoundDevice(SoundDevice* sDevice);

	//Library setter
	void setAssetLibrary(AssetLibrary* Library);
	
	//Other setters
	void setFPS(GAME_INT fps){ GAME_FPS = fps;}
	void setCityCorner(GAME_VEC cityCorner){this -> cityCorner = cityCorner;}
	void setLevel(GAME_LEVEL level){this -> level = level;}
	void setLoadBasement(bool loadBasement){this -> loadBasement = loadBasement;}

protected:
	//Devices
	std::unique_ptr<GraphicsDevice> gDevice;
	std::unique_ptr<InputDevice> iDevice;
	std::unique_ptr<PhysicsDevice> pDevice;
	std::unique_ptr<SoundDevice> sDevice;

	//LIbrary
	std::unique_ptr<AssetLibrary> assetLibrary;


	std::unique_ptr<ObjectFactory> factory;
	
	GAME_INT GAME_FPS;
	GAME_VEC cityCorner;
	GAME_LEVEL level;
	bool loadBasement;
};

#endif