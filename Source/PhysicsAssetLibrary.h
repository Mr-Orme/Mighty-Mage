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
	GAME_PHYSICS Search(std::string name);
	//bool Initialize(std::string path);
	bool Initialize();
	bool AddAsset(std::string name, GAME_PHYSICS physics);

protected:
	std::map<std::string, GAME_PHYSICS> library;
	

private:
};


#endif