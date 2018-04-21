
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
	//Initialize the random number generator
	//========================================
	srand((unsigned int)time(NULL));
	
	//========================================
	//Construct Game
	//========================================
	std::shared_ptr<Game> game = std::make_shared<Game>();
	
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
		game = NULL;
	}

	

	return 0;

}