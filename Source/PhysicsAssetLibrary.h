#ifndef PHYSICSASSETLIBRARY_H
#define PHYSICSASSETLIBRARY_H

#include <string>
#include <map>
#include <iostream>
#include "Definitions.h"

class PhysicsAssetLibrary{
public:
	void Print();
			//Asset Name, Asset Factory
	PhysicsStats Search(std::string name);
	//bool initialize(std::string path);
	bool initialize();
	bool AddAsset(std::string name, PhysicsStats physics);

protected:
	std::map<std::string, PhysicsStats> library;
	

private:
};


#endif