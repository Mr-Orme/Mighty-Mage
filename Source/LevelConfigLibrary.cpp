#include "LevelConfigLibrary.h"

LevelConfigLibrary::LevelConfigLibrary(){}


std::pair<std::string, std::string> LevelConfigLibrary::search(Levels level)
{
	return(library.find(level) -> second);
}
bool LevelConfigLibrary::addAsset(Levels level, std::string path, std::string assets)
{
	library[level] = { path, assets };
	return true;
}
bool LevelConfigLibrary::removeAsset(Levels level)
{
	
	if(auto toDelete{ library.find(level) }; toDelete != library.end())
	{
		library.erase(toDelete);
		return true;
	}
	else
		return false;
}