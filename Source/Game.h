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
	void Update();
	void Draw();
	

	

private:
	std::vector<std::shared_ptr<GameObject>> objects;
	std::vector<std::shared_ptr<GameObject>> newObjects;
	

	std::shared_ptr<ResourceManager> devices;
	GAME_FLT gameTime;
	bool debug;
	GAME_INT SCREEN_WIDTH;
	GAME_INT SCREEN_HEIGHT;
	
	
};


#endif