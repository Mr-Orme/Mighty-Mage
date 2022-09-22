#include "ArtAssetLibrary.h"
#include "Texture.h"
#include "GraphicsDevice.h"
ArtAssetLibrary::ArtAssetLibrary(GraphicsDevice* gDevice):
	gDevice(gDevice)
{
	
}

//**************************************
//Adds a texture to the library based on the object's name and the path to it's sprite
bool ArtAssetLibrary::AddAsset(std::string name, std::string path)
//**************************************
{
	library.emplace(std::make_pair(name, std::make_unique<Texture>(path, gDevice) ));
	
	return true;
}

//**************************************
//This function takes a string and finds the appropriate art asset in the library
//it returns the art object's texture.
Texture* ArtAssetLibrary::Search(std::string searchString)

//**************************************
{
	return (library.find(searchString) ->second.get());
}
