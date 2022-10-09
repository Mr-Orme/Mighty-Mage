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
	const int pixelsPerSquare{ 110 };
	const int blocksPerMap{ 15 };
	const int FPS{ 100 };

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

	Levels getLevel() const {return level;}
	void setLevel(Levels level) { this->level = level; }
	void changeLevel(Levels exit) { this->toLoad = exit; }
	Levels level2Load() const { return toLoad; }	
	
	Vector2D pixel2Square(Vector2D position) const;
	/*
	* fractional part is percentage of square
	* for placing smaller objects inside a square.
	*/
	Vector2D square2Pixel(float x, float y) const; 
	Vector2D square2Pixel(Vector2D square, Vector2D fractional = { 0,0 }) const;	
	
	void pause() { paused = true; }
	void upause() { paused = false; }
	bool isPaused() { return paused; }
	

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
	
	
	Levels level{ Levels::sorpigal };
	Levels toLoad{ Levels::none };	
	bool paused{ false };
};

#endif