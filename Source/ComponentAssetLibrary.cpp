#include "ComponentAssetLibrary.h"
#include "ComponentsList.h"
#include "GameObject.h"


ComponentAssetLibrary::ComponentAssetLibrary(){}
//**************************************
//Nothing happening here, move along
bool ComponentAssetLibrary::Initialize()
//**************************************
{
	return true;
}

//**************************************
//Takes the name of the object and a pointer to it
//creates components based on it's type and adds them to a vector
//which is returned.
std::vector<std::shared_ptr<Component>> ComponentAssetLibrary::Search(std::string name, std::shared_ptr<GameObject> owner)
//**************************************
{
	//Vector of pointers to return
	std::vector<std::shared_ptr<Component>> componentListPtrs;
	
	//finds the list of components associated with the name of the object passed.
	std::vector<GAME_COMPONENT_LIST> componentList = library.find(name) -> second;

	//Iterate through the list of components
	//add the proper component to the list of components to return.
	for( auto comp: componentList)
	{
		switch (comp)
		{
		case GAME_BODY_COMP:
			componentListPtrs.push_back(std::make_shared<BodyComponent>(owner));
			break;
		case GAME_HEALTH_COMP:
			componentListPtrs.push_back(std::make_shared<HealthComponent>(owner));
			break;
		case GAME_RENDERER_COMP:
			componentListPtrs.push_back(std::make_shared<RendererComponent>(owner));
			break;
		case GAME_USERINPUT_COMP:
			componentListPtrs.push_back(std::make_shared<UserInputComponent>(owner));
			break;
		case GAME_BACKPACK_COMP:
			componentListPtrs.push_back(std::make_shared<BackpackComponent>(owner));
			break;
		case GAME_INVENTORY_COMP:
			componentListPtrs.push_back(std::make_shared<InventoryComponent>(owner));
			break;
		case GAME_GHOST_COMP:
			componentListPtrs.push_back(std::make_shared<GhostComponent>(owner));
			break;
		default:
			break;
		}
	}
	//return the completed list
	return componentListPtrs;
}

//**************************************
//takes the name of the object and a vecotr of enums of types of components and saves it to the library.
//The components cannot be created until there is an actual owner for it.
bool ComponentAssetLibrary::AddAsset(std::string name, std::vector<GAME_COMPONENT_LIST> componentList)
//**************************************
{
	library[name] = componentList;
	return true;
}