#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "tinyxml2.h"
#include "GraphicsDevice.h"
#include "InputDevice.h"
#include "PhysicsDevice.h"
#include "SoundDevice.h"
#include "ObjectLibrary.h"
#include "NoticesAssetLibrary.h"
#include "SoundAssetLibrary.h"
#include "View.h"


class ObjectFactory;
enum class Levels { sorpigal, sorpigal_basement, };
class ResourceManager
{
public:
	ResourceManager(Vector2D screenDimensions, std::string assetPath);
	~ResourceManager();

	GraphicsDevice* GetGraphicsDevice();
	InputDevice* GetInputDevice(){return iDevice.get();}
	PhysicsDevice* GetPhysicsDevice(){return pDevice.get();}
	SoundDevice* GetSoundDevice(){return sDevice.get();}

	ObjectLibrary* GetObjectLibrary(){return oLibrary.get();}
	NoticesAssetLibrary* GetNoticesLibrary(){return nLibrary.get();}
	SoundAssetLibrary* GetSoundLibrary(){return sLibrary.get();}

	//ObjectFactor Getter
	ObjectFactory* GetObjectFactory() {return factory.get();}

	//Other Getters
	int GetFPS(){return FPS;}
	Vector2D GetCityCorner(){return cityCorner;}
	Levels GetLevel(){return level;}
	bool GetLoadBasement(){return loadBasement;}
	GameObject* getExit() { return levelExit; }
	

	//Other Setters
	void SetFPS(int fps){ FPS = fps;}
	void SetCityCorner(Vector2D cityCorner){this -> cityCorner = cityCorner;}
	void SetLevel(Levels level){this -> level = level;}
	void SetLoadBasement(bool loadBasement){this -> loadBasement = loadBasement;}
	void setExit(GameObject* levelExit) { this->levelExit = levelExit; }

	bool isExitSquare(Vector2D currSquare) const;
	const int pixelsPerSquare{ 110 };
	const int blocksPerMap{ 15 };
protected:
	void loadLibraries(std::string assetPath);
	void populateComponentLibrary(tinyxml2::XMLElement* asset);
	//Devices
	std::unique_ptr<GraphicsDevice> gDevice{ nullptr };
	std::unique_ptr<InputDevice> iDevice{ nullptr };
	std::unique_ptr<PhysicsDevice> pDevice{ nullptr };
	std::unique_ptr<SoundDevice> sDevice{ nullptr };

	//Libraries
	std::unique_ptr<ObjectLibrary> oLibrary{ nullptr };
	std::unique_ptr<NoticesAssetLibrary> nLibrary{ nullptr };
	std::unique_ptr<SoundAssetLibrary> sLibrary{ nullptr };

	std::unique_ptr<ObjectFactory> factory{ nullptr };
	
	int FPS{ 100 };
	Vector2D cityCorner{ 0,0 };
	Levels level{ Levels::sorpigal };
	bool loadBasement{ false };
	std::vector<Vector2D> exits;

	GameObject* levelExit{ nullptr };

	
};

#endif