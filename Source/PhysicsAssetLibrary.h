#ifndef PHYSICSASSETLIBRARY_H
#define PHYSICSASSETLIBRARY_H

#include <string>
#include <map>
#include <iostream>
#include "Definitions.h"
#include "PhysicsDevice.h"

class PhysicsAssetLibrary{
public:
	void Print();
			//Asset Name, Asset Factory
	PhysicsDevice::GAME_PHYSICS Search(std::string name);
	//bool Initialize(std::string path);
	bool Initialize();
	bool AddAsset(std::string name, PhysicsDevice::GAME_PHYSICS physics);

protected:
	std::map<std::string, PhysicsDevice::GAME_PHYSICS> library;
	

private:
};


#endif