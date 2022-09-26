#ifndef OBJECTASSETLIBRARY_H
#define OBJECTASSETLIBRARY_H

#include <map>
#include <string>
#include <memory>
#include <vector>



class GameObject;

	
class ObjectAssetLibrary{
public:
	ObjectAssetLibrary();
	ObjectStats search(std::string name);
	bool addAsset(std::string name, ObjectStats stats);
private:
			//name					List of components
	std::map<std::string, ObjectStats> library;


};


#endif