#ifndef ARTASSETLIBRARY_H
#define ARTASSETLIBRARY_H

#include <map>
#include <string>
#include <memory>


class Texture;
class GraphicsDevice;

class ArtAssetLibrary{
public:
	ArtAssetLibrary(GraphicsDevice* gDevice);
	Texture* search(std::string);
	bool addAsset(std::string name, std::string path);
private:
	std::map<std::string, std::unique_ptr<Texture>> library;
	GraphicsDevice* gDevice;

};


#endif