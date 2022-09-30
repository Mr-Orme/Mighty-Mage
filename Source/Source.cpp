
#include <iostream>
#include <ctime>
#include <memory>

#include "Game.h"

int main(int argc, char *argv[])
{
	std::unique_ptr<Game> game{ std::make_unique<Game>() };
	while(game->run()){}
	
	return 0;

}