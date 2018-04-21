#ifndef COMPONENTASSETLIBRARY_H
#define COMPONENTASSETLIBRARY_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Definitions.h"

class Component;
class GameObject;

	
class ComponentAssetLibrary{
public:

	ComponentAssetLibrary();
	bool Initialize();

	enum GAME_COMPONENT_LIST
	{
		GAME_BODY_COMP,
		GAME_HEALTH_COMP,
		GAME_RENDERER_COMP,
		GAME_USERINPUT_COMP,
		GAME_BACKPACK_COMP,
		GAME_INVENTORY_COMP,
		GAME_GHOST_COMP,
	};
	std::vector<std::shared_ptr<Component>> Search(std::string name, std::shared_ptr<GameObject> owner);
	bool AddAsset(std::string name, std::vector<GAME_COMPONENT_LIST> componentList);
private:
			//name					List of components
	std::map<std::string, std::vector<GAME_COMPONENT_LIST>> library;


};


#endif