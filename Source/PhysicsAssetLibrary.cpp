#include "PhysicsAssetLibrary.h"
#include "tinyxml\tinyxml.h"

//**************************************
//Finds an item in the library and returns the physics struct
//associated with it.
PhysicsDevice::GAME_PHYSICS PhysicsAssetLibrary::Search(std::string searchText)
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
bool PhysicsAssetLibrary::AddAsset(std::string name, PhysicsDevice::GAME_PHYSICS physics)
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
	for(auto asset : library)
	{
		std::cout << 
			"**********************" <<asset.first << "***************************" << std::endl <<
			"Angular Damping: " <<asset.second.angularDamping  << std::endl <<
			"Density: " <<asset.second.density  << std::endl <<
			"Linear Damping: " <<asset.second.linearDamping << std::endl <<
			"Force: " <<asset.second.force << std::endl <<
			"Friction: " <<asset.second.friction << std::endl <<		
			"Object Shape: " <<asset.second.objectShape << std::endl <<
			"Spin Speed: " <<asset.second.spinSpeed  << std::endl <<
			std::endl;
	}
}

