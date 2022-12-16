#include "ObjectLibrary.h"
#include "GameObject.h"


ObjectLibrary::ObjectLibrary(){}


//**************************************
//Takes the name of the object and returns it's stats.
//TODO::Use std::optional in case it's not found!
ObjectDefinition&  ObjectLibrary::search(std::string name)
//**************************************
{
	return (library.find(name) -> second);
}

//**************************************
//takes the name of the object and a vecotr of enums of types of stats and saves it to the library.
bool ObjectLibrary::addAsset(ID id, ObjectDefinition stats)
//**************************************
{
	stats.id = id;
	library[id.name] = std::move(stats);
	return true;
}