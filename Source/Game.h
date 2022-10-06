#ifndef GAME_H
#define GAME_H

#include "Vector2D.h"
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
class Game{
public:
	
	Game();
	~Game();
	
	void reset();
	bool loadLevel(Levels level);
	bool run();
	std::optional<Levels> update();
	
private:
	bool loadExtras(tinyxml2::XMLElement* squareElement);
	
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::unique_ptr<GameObject>> newObjects;
	
	std::unique_ptr<ResourceManager> devices{ nullptr };
	float gameTime{};
	bool debug{ false };
	Vector2D screenDimensions{800, 600};
	std::unique_ptr<Timer> frameTimer{ nullptr };
	std::unique_ptr<LevelConfigLibrary> levelLibrary{nullptr};
	int squareDimension{ 110 };
};


#endif