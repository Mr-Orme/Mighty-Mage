#ifndef GAME_H
#define GAME_H

#include "Vector2D.h"
#include "Definitions.h"
#include "tinyxml2.h"
#include <string>
#include <vector>
#include <memory>
#include <optional>

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
	void loadLevelLibrary(std::string levelFile);
	ObjectFactoryPresets loadExtras(tinyxml2::XMLElement* squareElement, std::optional<Vector2D> playerStart = std::nullopt, std::optional<Direction> playerDirection = Direction::N);
	std::optional<ObjectFactoryPresets> loadLeftWall(tinyxml2::XMLElement* squareElement, Vector2D square);
	ObjectFactoryPresets loadTopWall(tinyxml2::XMLElement* squareElement, Vector2D square);
	std::optional<ObjectFactoryPresets> loadFloor (tinyxml2::XMLElement* squareElement, Vector2D square);
	
	bool parseLevelXML(std::string levelConfig, std::optional<Vector2D> playerStart, std::optional<Direction> playerDirection);
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