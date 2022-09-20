#include "ObjectAssetLibrary.h"
#include "GameObject.h"


ObjectAssetLibrary::ObjectAssetLibrary(){}
//**************************************
//Nothing happening here, move along
bool ObjectAssetLibrary::initialize()
//**************************************
{
	return true;
}

//**************************************
//Takes the name of the object and returns it's stats.
//right now it is just health, but that will change.
ObjectStats  ObjectAssetLibrary::Search(std::string name)
//**************************************
{
	return (library.find(name) -> second);
}

//**************************************
//takes the name of the object and a vecotr of enums of types of stats and saves it to the library.
bool ObjectAssetLibrary::AddAsset(std::string name, ObjectStats stats)
//**************************************
{
	library[name] = stats;
	return true;
}