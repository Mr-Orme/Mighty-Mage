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

class Game{
public:
	
	Game();
	~Game();
	bool Initialize ();
	void Reset();
	bool LoadLevel(std::string levelConfigFile, std::string assetConfigFile);
	bool Run();
	void update();
	void Draw();
	

	

private:
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::unique_ptr<GameObject>> newObjects;
	

	std::unique_ptr<ResourceManager> devices;
	float gameTime;
	bool debug;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	
	
};


#endif