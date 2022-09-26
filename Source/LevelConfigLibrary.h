#ifndef LEVELCONFIGLIBRARY_H
#define LEVELCONFIGLIBRARY_H

#include <map>
#include <string>

//Keeps track of paths to level xml files
//TODO::implement this library.
class LevelConfigLibrary{
public:
	LevelConfigLibrary();
	std::string search(std::string name);
	bool addAsset(std::string name, std::string path);
	bool removeAsset(std::string name);
private:
	std::map<std::string, std::string> library;
};


#endif