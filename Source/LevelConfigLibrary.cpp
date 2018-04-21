#include "LevelConfigLibrary.h"

LevelConfigLibrary::LevelConfigLibrary(){}
bool LevelConfigLibrary::Initialize(){return true;}
std::string LevelConfigLibrary::Search(std::string name)
{
	return(library.find(name) -> second);
}
bool LevelConfigLibrary::AddAsset(std::string name, std::string path)
{
	library[name]=path;
	return true;
}
bool LevelConfigLibrary::RemoveAsset(std::string name)
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