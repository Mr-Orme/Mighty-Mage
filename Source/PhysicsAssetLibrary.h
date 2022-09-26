#ifndef PHYSICSASSETLIBRARY_H
#define PHYSICSASSETLIBRARY_H

#include <string>
#include <map>
#include <iostream>


class PhysicsAssetLibrary{
public:
			//TODO::try and combine a bunch of libraries...
	PhysicsStats search(std::string name);
	bool addAsset(std::string name, PhysicsStats physics);

protected:
	std::map<std::string, PhysicsStats> library;
	

private:
};


#endif