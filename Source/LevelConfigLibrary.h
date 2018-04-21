#ifndef LEVELCONFIGLIBRARY_H
#define LEVELCONFIGLIBRARY_H

#include <map>
#include <string>
#include "Definitions.h"

class LevelConfigLibrary{
public:
	LevelConfigLibrary();
	bool Initialize();
	std::string Search(std::string name);
	bool AddAsset(std::string name, std::string path);
	bool RemoveAsset(std::string name);
private:
	std::map<std::string, std::string> library;
};


#endif