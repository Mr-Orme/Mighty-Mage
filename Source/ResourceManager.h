#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Definitions.h"

class ObjectFactory;

class ComponentAssetLibrary;
class ObjectAssetLibrary;
class NoticesAssetLibrary;
class SoundAssetLibrary;
class PhysicsAssetLibrary;
class ArtAssetLibrary;

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

	//Device Getters
	GraphicsDevice* GetGraphicsDevice();
	InputDevice* GetInputDevice();
	PhysicsDevice* GetPhysicsDevice();
	SoundDevice* GetSoundDevice();

	//Device Setters
	ArtAssetLibrary* GetArtLibrary();
	PhysicsAssetLibrary* GetPhysicsLibrary();
	ComponentAssetLibrary* GetComponentLibrary();
	ObjectAssetLibrary* GetObjectLibrary();
	NoticesAssetLibrary* GetNoticesLibrary();
	SoundAssetLibrary* GetSoundLibrary();

	//ObjectFactor Getter
	ObjectFactory* GetObjectFactory();

	//Other Getters
	GAME_INT GetFPS(){return GAME_FPS;}
	GAME_VEC GetCityCorner(){return cityCorner;}
	GAME_LEVEL GetLevel(){return level;}
	bool GetLoadBasement(){return loadBasement;}

	//Device Setters
	void SetGraphicsDevice(GraphicsDevice* gDevice);
	void SetInputDevice(InputDevice* iDevice);
	void SetPhysicsDevice(PhysicsDevice* pDevice);
	void SetSoundDevice(SoundDevice* sDevice);

	//Library Setters
	void SetArtLibrary(ArtAssetLibrary* aLibrary);
	void SetPhysicsLibrary(PhysicsAssetLibrary* pLibrary);
	void SetComponentLibrary(ComponentAssetLibrary* cLibrary);
	void SetObjectLibrary(ObjectAssetLibrary* oLibrary);
	void SetNoticesLibrary(NoticesAssetLibrary* nLibrary);
	void SetSoundLibrary(SoundAssetLibrary* sLibrary);

	//Other Setters
	void SetFPS(GAME_INT fps){ GAME_FPS = fps;}
	void SetCityCorner(GAME_VEC cityCorner){this -> cityCorner = cityCorner;}
	void SetLevel(GAME_LEVEL level){this -> level = level;}
	void SetLoadBasement(bool loadBasement){this -> loadBasement = loadBasement;}

protected:
	//Devices
	std::unique_ptr<GraphicsDevice> gDevice;
	std::unique_ptr<InputDevice> iDevice;
	std::unique_ptr<PhysicsDevice> pDevice;
	std::unique_ptr<SoundDevice> sDevice;

	//LIbraries
	std::unique_ptr<ArtAssetLibrary> aLibrary;
	std::unique_ptr<PhysicsAssetLibrary> pLibrary;
	std::unique_ptr<ComponentAssetLibrary> cLibrary;
	std::unique_ptr<ObjectAssetLibrary> oLibrary;
	std::unique_ptr<NoticesAssetLibrary> nLibrary;
	std::unique_ptr<SoundAssetLibrary> sLibrary;

	std::unique_ptr<ObjectFactory> factory;
	
	GAME_INT GAME_FPS;
	GAME_VEC cityCorner;
	GAME_LEVEL level;
	bool loadBasement;
};

#endif