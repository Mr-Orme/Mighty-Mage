#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "tinyxml2.h"

#include "Definitions.h"
#include "Vector2D.h"
#include "ObjectLibrary.h"


class View;
class SoundAssetLibrary;
class NoticesAssetLibrary;
class ObjectLibrary;
class SoundDevice;
class PhysicsDevice;
class InputDevice;
class GraphicsDevice;
class ObjectFactory;
class GameObject;

enum class Levels { sorpigal, sorpigal_basement, none};
class ResourceManager
{
public:
	ResourceManager(Vector2D screenDimensions, std::string assetPath);
	~ResourceManager();

	GraphicsDevice* getGraphicsDevice();
	InputDevice* getInputDevice();
	PhysicsDevice* getPhysicsDevice();
	SoundDevice* getSoundDevice();

	ObjectLibrary* getObjectLibrary();
	NoticesAssetLibrary* getNoticesLibrary();
	SoundAssetLibrary* getSoundLibrary();

	ObjectFactory* getObjectFactory();

	int getFPS() const {return FPS;}
	Vector2D getCityCorner() const {return cityCorner;}
	Levels getLevel() const {return level;}
	Levels level2Load() const { return toLoad; }
	bool isPaused() { return paused; }

	void setCityCorner(Vector2D cityCorner){this -> cityCorner = cityCorner;}
	void setLevel(Levels level){this -> level = level;}
	void changeLevel(Levels exit) { this->toLoad = exit; }
	void pause() { paused = true; }
	void upause() { paused = false; }
	
	const int pixelsPerSquare{ 110 };
	const int blocksPerMap{ 15 };
private:
	void loadLibraries(std::string assetPath);
	void loadNotices(tinyxml2::XMLElement* notices);
	void loadObjects(tinyxml2::XMLElement* asset);
	void loadSounds(tinyxml2::XMLElement* sounds);

	ObjectDefinition loadComponent(tinyxml2::XMLElement* component);


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
	
	const int FPS{ 100 };
	//TODO::get rid of this by makiing it 0,0 and adjust the view to center player.
	Vector2D cityCorner{ 0,0 };//relative to player's start position.
	Levels level{ Levels::sorpigal };
	Levels toLoad{ Levels::none };	
	bool paused{ false };
};

#endif