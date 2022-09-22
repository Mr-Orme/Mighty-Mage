#include "LevelConfigLibrary.h"

LevelConfigLibrary::LevelConfigLibrary(){}


std::string LevelConfigLibrary::search(std::string name)
{
	return(library.find(name) -> second);
}
bool LevelConfigLibrary::addAsset(std::string name, std::string path)
{
	library[name]=path;
	return true;
}
bool LevelConfigLibrary::removeAsset(std::string name)
{
	std::map<std::string, std::string>::iterator toDelete;
	toDelete = library.find(name);
	if(toDelete != library.end())
	{
		library.erase(library.find(name));
		return true;
	}
	else
		return false;
}