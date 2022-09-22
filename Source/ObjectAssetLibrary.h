#ifndef OBJECTASSETLIBRARY_H
#define OBJECTASSETLIBRARY_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Definitions.h"


class GameObject;

	
class ObjectAssetLibrary{
public:
	ObjectAssetLibrary();
	ObjectStats Search(std::string name);
	bool AddAsset(std::string name, ObjectStats stats);
private:
			//name					List of components
	std::map<std::string, ObjectStats> library;


};


#endif