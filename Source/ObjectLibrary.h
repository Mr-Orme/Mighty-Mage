#ifndef OBJECTASSETLIBRARY_H
#define OBJECTASSETLIBRARY_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Initializers.h"
#include "Texture.h"

class GameObject;
class Component;

struct ObjectDefinition
{
	ID id;
	std::unique_ptr<Texture> sprite;
	PhysicsStats physics;
	std::vector<std::unique_ptr<Component>> components;
};
class ObjectLibrary{
public:
	ObjectLibrary();
	ObjectDefinition& search(std::string name);
	bool addAsset(ID id, ObjectDefinition stats);
private:
	std::map<std::string, ObjectDefinition> library;


};


#endif