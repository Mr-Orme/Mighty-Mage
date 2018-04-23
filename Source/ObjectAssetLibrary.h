#ifndef OBJECTASSETLIBRARY_H
#define OBJECTASSETLIBRARY_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Definitions.h"


class GameObject;
#include "ObjectFactory.h"
	
class ObjectAssetLibrary{
public:
	ObjectAssetLibrary();
	bool Initialize();
	ObjectFactory::GAME_OBJECT_STATS Search(std::string name);
	bool AddAsset(std::string name, ObjectFactory::GAME_OBJECT_STATS stats);
private:
			//name					List of components
	std::map<std::string, ObjectFactory::GAME_OBJECT_STATS> library;


};


#endif