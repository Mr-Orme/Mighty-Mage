#ifndef MIGHTYMAGE_GAME_H
#define MIGHTYMAGE_GAME_H

#include <string>
#include <vector>
#include <memory>
#include <optional>

#include "tinyxml2.h"

#include "Vector2D.h"
#include "Definitions.h"



class Timer;
class ResourceManager;
class ObjectFactory;
class GameObject;
class LevelConfigLibrary;
enum class Levels;
struct ObjectFactoryPresets;
class Game{
public:
	
	Game();
	~Game();
	
	void reset();
	bool loadLevel(Levels level);
	bool run();
	bool update();
	
private:
	ObjectFactoryPresets loadExtras(tinyxml2::XMLElement* squareElement, ResourceManager* devices, std::optional<Vector2D> playerStart = std::nullopt, std::optional<Direction> playerDirection = Direction::N);
	std::optional<ObjectFactoryPresets> loadLeftWall(tinyxml2::XMLElement* squareElement, Vector2D square, ResourceManager* devices);
	ObjectFactoryPresets loadTopWall(tinyxml2::XMLElement* squareElement, Vector2D square, ResourceManager* devices);
	std::optional<ObjectFactoryPresets> loadFloor (tinyxml2::XMLElement* squareElement, Vector2D square, ResourceManager* devices);
	
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::unique_ptr<GameObject>> newObjects;
	
	std::unique_ptr<ResourceManager> devices{ nullptr };
	float gameTime{};
	bool debug{ false };
	Vector2D screenDimensions{800, 600};
	std::unique_ptr<Timer> frameTimer{ nullptr };
	std::unique_ptr<LevelConfigLibrary> levelLibrary{nullptr};
};


#endif