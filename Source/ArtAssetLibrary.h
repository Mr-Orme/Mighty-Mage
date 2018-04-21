#ifndef ARTASSETLIBRARY_H
#define ARTASSETLIBRARY_H

#include <map>
#include <string>


class Texture;
class GraphicsDevice;

class ArtAssetLibrary{
public:
	ArtAssetLibrary();
	bool Initialize(GraphicsDevice* gDevice);
	Texture* Search(std::string);
	bool AddAsset(std::string name, std::string path);
private:
	std::map<std::string, Texture*> library;
	GraphicsDevice* gDevice;

};


#endif