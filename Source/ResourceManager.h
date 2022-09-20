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
	ResourceManager(Vector2D screenDimensions, std::string assetPath);
	~ResourceManager();
	
	bool initialize(Vector2D screenDimensions, std::string assetPath);

	//Device Getters
	GraphicsDevice* GetGraphicsDevice(){return gDevice.get();}
	InputDevice* GetInputDevice(){return iDevice.get();}
	PhysicsDevice* GetPhysicsDevice(){return pDevice.get();}
	SoundDevice* GetSoundDevice(){return sDevice.get();}

	//Device Setters
	ArtAssetLibrary* GetArtLibrary(){return aLibrary.get();}
	PhysicsAssetLibrary* GetPhysicsLibrary(){return pLibrary.get();}
	ComponentAssetLibrary* getComponentLibrary(){return cLibrary.get();}
	ObjectAssetLibrary* GetObjectLibrary(){return oLibrary.get();}
	NoticesAssetLibrary* GetNoticesLibrary(){return nLibrary.get();}
	SoundAssetLibrary* GetSoundLibrary(){return sLibrary.get();}

	//ObjectFactor Getter
	ObjectFactory* GetObjectFactory() {return factory.get();}

	//Other Getters
	int GetFPS(){return FPS;}
	Vector2D GetCityCorner(){return cityCorner;}
	Levels GetLevel(){return level;}
	bool GetLoadBasement(){return loadBasement;}

	

	//Other Setters
	/*void SetFPS(int fps){ FPS = fps;}
	void SetCityCorner(Vector2D cityCorner){this -> cityCorner = cityCorner;}
	void SetLevel(Levels level){this -> level = level;}
	void SetLoadBasement(bool loadBasement){this -> loadBasement = loadBasement;}*/

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
	
	int FPS{ 100 };
	Vector2D cityCorner;
	Levels level;
	bool loadBasement;
};

#endif