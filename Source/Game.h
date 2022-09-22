#ifndef GAME_H
#define GAME_H

#include "ResourceManager.h"

#include "ArtAssetLibrary.h"
#include "PhysicsAssetLibrary.h"

#include "ObjectFactory.h"

#include "View.h"
#include "Timer.h"

#include "Definitions.h"
#include "tinyxml\tinyxml.h"

#include <string>
#include <vector>
#include <memory>

class Game{
public:
	
	Game();
	Game(std::string levelConfigFile, std::string assetConfigFile);
	~Game();
	
	void reset();
	bool loadLevel(std::string levelConfigFile, std::string assetConfigFile);
	bool run();
	void update();
	

	

private:
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::unique_ptr<GameObject>> newObjects;
	

	std::unique_ptr<ResourceManager> devices{ nullptr };
	float gameTime{};
	bool debug{ false };
	Vector2D screenDimensions{800, 600};
	
	
};


#endif