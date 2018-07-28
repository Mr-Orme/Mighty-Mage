#ifndef GAME_H
#define GAME_H

#include "Definitions.h"

#include <string>
#include <vector>

class GameObject;
class ResourceManager;

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
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::unique_ptr<GameObject>> newObjects;
	

	std::unique_ptr<ResourceManager> devices;
	GAME_FLT gameTime;
	bool debug;
	GAME_INT SCREEN_WIDTH;
	GAME_INT SCREEN_HEIGHT;
	
	
};


#endif