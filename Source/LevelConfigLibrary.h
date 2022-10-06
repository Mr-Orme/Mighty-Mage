#ifndef LEVELCONFIGLIBRARY_H
#define LEVELCONFIGLIBRARY_H

#include <map>
#include <string>

//Keeps track of paths to level xml files
enum class Levels;
class LevelConfigLibrary{
public:
	LevelConfigLibrary();
	std::pair<std::string, std::string> search(Levels level);
	bool addAsset(Levels level, std::string path, std::string assets);
	bool removeAsset(Levels level);
private:							//path, assets
	std::map<Levels, std::pair<std::string, std::string> > library;
};


#endif