#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "GraphicsDevice.h"
#include "InputDevice.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"
#include "ArtAssetLibrary.h"
#include "PhysicsAssetLibrary.h"
#include "ComponentAssetLibrary.h"
#include "ObjectAssetLibrary.h"
#include "NoticesAssetLibrary.h"
#include "SoundAssetLibrary.h"
#include "View.h"

#include "Definitions.h"
class ObjectFactory;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	
	bool Initialize(GAME_INT SCREEN_WIDTH, GAME_INT SCREEN_HEIGHT, std::string assetPath);
	bool Shutdown();

	//Device Getters
	GraphicsDevice* GetGraphicsDevice(){return gDevice.get();}
	InputDevice* GetInputDevice(){return iDevice.get();}
	PhysicsDevice* GetPhysicsDevice(){return pDevice.get();}
	SoundDevice* GetSoundDevice(){return sDevice.get();}

	//Device Setters
	ArtAssetLibrary* GetArtLibrary(){return aLibrary.get();}
	PhysicsAssetLibrary* GetPhysicsLibrary(){return pLibrary.get();}
	ComponentAssetLibrary* GetComponentLibrary(){return cLibrary.get();}
	ObjectAssetLibrary* GetObjectLibrary(){return oLibrary.get();}
	NoticesAssetLibrary* GetNoticesLibrary(){return nLibrary.get();}
	SoundAssetLibrary* GetSoundLibrary(){return sLibrary.get();}

	//ObjectFactor Getter
	ObjectFactory* GetObjectFactory() {return factory.get();}

	//Other Getters
	GAME_INT GetFPS(){return GAME_FPS;}
	GAME_VEC GetCityCorner(){return cityCorner;}
	GAME_LEVEL GetLevel(){return level;}
	bool GetLoadBasement(){return loadBasement;}

	//Device Setters
	void SetGraphicsDevice(std::shared_ptr<GraphicsDevice> gDevice){this -> gDevice = gDevice;}
	void SetInputDevice(std::shared_ptr<InputDevice> iDevice){this -> iDevice = iDevice;}
	void SetPhysicsDevice(std::shared_ptr<PhysicsDevice> pDevice){this -> pDevice = pDevice;}
	void SetSoundDevice(std::shared_ptr<SoundDevice> sDevice){this -> sDevice = sDevice;}

	//Library Setters
	void SetArtLibrary(std::shared_ptr<ArtAssetLibrary> aLibrary){this -> aLibrary = aLibrary;}
	void SetPhysicsLibrary(std::shared_ptr<PhysicsAssetLibrary> pLibrary){this -> pLibrary = pLibrary;}
	void SetComponentLibrary(std::shared_ptr<ComponentAssetLibrary> cLibrary){this -> cLibrary = cLibrary;}
	void SetObjectLibrary(std::shared_ptr<ObjectAssetLibrary> oLibrary){this -> oLibrary = oLibrary;}
	void SetNoticesLibrary(std::shared_ptr<NoticesAssetLibrary> nLibrary){this -> nLibrary = nLibrary;}
	void SetSoundLibrary(std::shared_ptr<SoundAssetLibrary> sLibrary){this -> sLibrary = sLibrary;}

	//Other Setters
	void SetFPS(GAME_INT fps){ GAME_FPS = fps;}
	void SetCityCorner(GAME_VEC cityCorner){this -> cityCorner = cityCorner;}
	void SetLevel(GAME_LEVEL level){this -> level = level;}
	void SetLoadBasement(bool loadBasement){this -> loadBasement = loadBasement;}

protected:
	//Devices
	std::shared_ptr<GraphicsDevice> gDevice;
	std::shared_ptr<InputDevice> iDevice;
	std::shared_ptr<PhysicsDevice> pDevice;
	std::shared_ptr<SoundDevice> sDevice;

	//LIbraries
	std::shared_ptr<ArtAssetLibrary> aLibrary;
	std::shared_ptr<PhysicsAssetLibrary> pLibrary;
	std::shared_ptr<ComponentAssetLibrary> cLibrary;
	std::shared_ptr<ObjectAssetLibrary> oLibrary;
	std::shared_ptr<NoticesAssetLibrary> nLibrary;
	std::shared_ptr<SoundAssetLibrary> sLibrary;

	std::shared_ptr<ObjectFactory> factory;
	
	GAME_INT GAME_FPS;
	GAME_VEC cityCorner;
	GAME_LEVEL level;
	bool loadBasement;
};

#endif