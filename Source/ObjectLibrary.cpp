#include "ObjectLibrary.h"
#include "GameObject.h"


ObjectLibrary::ObjectLibrary(){}


//**************************************
//Takes the name of the object and returns it's stats.
//right now it is just health, but that will change.
ObjectDefinition&  ObjectLibrary::search(std::string name)
//**************************************
{
	return (library.find(name) -> second);
}

//**************************************
//takes the name of the object and a vecotr of enums of types of stats and saves it to the library.
bool ObjectLibrary::addAsset(std::string name, ObjectDefinition stats)
//**************************************
{
	library[name] = std::move(stats);
	return true;
}