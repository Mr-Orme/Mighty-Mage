#include "ArtAssetLibrary.h"
#include "Texture.h"
#include "GraphicsDevice.h"
ArtAssetLibrary::ArtAssetLibrary()
{
	
}

//**************************************
//This function sets the graphic's device for the library.
bool ArtAssetLibrary::Initialize(GraphicsDevice* gDevice)

//**************************************
{
	this -> gDevice = gDevice;
	return true;
}

//**************************************
//Adds a texture to the library based on the object's name and the path to it's sprite
bool ArtAssetLibrary::AddAsset(std::string name, std::string path)
//**************************************
{
	//Texture variable
	Texture* tempTexture;
	
	//Make a new texture and assign
	tempTexture = new Texture();
	if(!tempTexture -> load(gDevice -> GetRenderer(), path)){return false;}
	//add to library
	library[name] = tempTexture;
	return true;
}

//**************************************
//This function takes a string and finds the appropriate art asset in the library
//it returns the art object's texture.
Texture* ArtAssetLibrary::Search(std::string searchString)

//**************************************
{
	return (library.find(searchString) ->second);
}
