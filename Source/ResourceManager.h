#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
//#include "GraphicsDevice.h"
//#include "InputDevice.h"
//#include "PhysicsDevice.h"
//#include "SoundDevice.h"
//#include "ArtAssetLibrary.h"
//#include "PhysicsAssetLibrary.h"
//#include "ComponentAssetLibrary.h"
//#include "ObjectAssetLibrary.h"
//#include "NoticesAssetLibrary.h"
//#include "SoundAssetLibrary.h"
//#include "View.h"
#include <memory>
#include <string>
#include "Definitions.h"
class ObjectFactory;
class GraphicsDevice;
class Blackboard;
class View;
class SoundAssetLibrary;
class NoticesAssetLibrary;
class ObjectAssetLibrary;
class ComponentAssetLibrary;
class PhysicsAssetLibrary;
class ArtAssetLibrary;
class SoundDevice;
class PhysicsDevice;
class InputDevice;

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
	Blackboard* getBlackboard() { return blackboard.get(); }

	//ObjectFactor Getter
	ObjectFactory* GetObjectFactory() {return factory.get();}

	//Other Getters
	GAME_INT GetFPS(){return GAME_FPS;}
	GAME_VEC GetCityCorner(){return cityCorner;}
	GAME_LEVEL GetLevel(){return level;}
	bool GetLoadBasement(){return loadBasement;}

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
	std::unique_ptr<Blackboard> blackboard;

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