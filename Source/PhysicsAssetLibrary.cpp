#include "PhysicsAssetLibrary.h"
#include "tinyxml\tinyxml.h"

//**************************************
//Finds an item in the library and returns the physics struct
//associated with it.
GAME_PHYSICS PhysicsAssetLibrary::Search(std::string searchText)
//**************************************
{
	return(library.find(searchText) -> second);
}

//**************************************
//Nothing here, move along.
bool PhysicsAssetLibrary::Initialize()
//**************************************
{
	return true;
}

//**************************************
//the name of the object type and a physics structured is passed in and saved to the library.
bool PhysicsAssetLibrary::AddAsset(std::string name, GAME_PHYSICS physics)
//**************************************
{
	//this game never has friction.
	physics.friction = 0.0f;
	library[name] = physics;
	return true;
}

//**************************************
//Prints items in the library
void PhysicsAssetLibrary::Print()
//**************************************
{
	std::map<std::string, GAME_PHYSICS>::iterator galIter;
	
	for(galIter = library.begin(); galIter != library.end(); galIter++)
	{
	
		
		std::cout << 
			"**********************" << galIter -> first << "***************************" << std::endl <<
			"Angular Damping: " << galIter -> second.angularDamping  << std::endl <<
			"Density: " << galIter -> second.density  << std::endl <<
			"Linear Damping: " << galIter -> second.linearDamping << std::endl <<
			"Force: " << galIter -> second.force << std::endl <<
			"Friction: " << galIter -> second.friction << std::endl <<		
			"Object Shape: " << galIter -> second.objectShape << std::endl <<
			"Spin Speed: " << galIter -> second.spinSpeed  << std::endl <<
			std::endl;
	}
}

