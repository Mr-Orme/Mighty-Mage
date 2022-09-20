
//C++ Libraries
#include <iostream>
#include <ctime>
#include <memory>

//Media Library Wrappers
#include "ResourceManager.h"

//Game
#include "Game.h"

//constants
#include "Definitions.h"

int main(int argc, char *argv[])
{
	
	std::string assetFile = "./Assets/Config/MainAssets.xml";
	std::string levelConfigFile = "./Assets/Config/MainLevel.xml";

	std::unique_ptr<Game> game{ std::make_unique<Game>(levelConfigFile, assetFile) };

	//While the user hasn't quit
	while(game->run()){}
	
	return 0;

}