
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


	
	//========================================
	//Construct Game
	//========================================
	std::unique_ptr<Game> game = std::make_unique<Game>();
	
	if(!game->Initialize())
	{
		printf( "Game could not Initialize!");
		exit(1);
	}
	
	//========================================
	//Load Level
	//========================================
	std::string assetFile= "./Assets/Config/MainAssets.xml";
	std::string levelConfigFile= "./Assets/Config/MainLevel.xml";
	if(!game->LoadLevel(levelConfigFile, assetFile))
	{
		printf( "Game could not load level %s: ", levelConfigFile);
		exit(1);
	}
	
	
	//========================================
	//Main Game Loop
	//========================================

	//While the user hasn't quit
	while(game->Run()){}
	

	//========================================
	// Clean-up
	//========================================
	if(game)
	{
		game = nullptr;
	}

	

	return 0;

}