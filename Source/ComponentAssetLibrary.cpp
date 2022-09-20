#include "ComponentAssetLibrary.h"
#include "ComponentsList.h"
#include "GameObject.h"


ComponentAssetLibrary::ComponentAssetLibrary(){}
//**************************************
//Nothing happening here, move along
bool ComponentAssetLibrary::initialize()
//**************************************
{
	return true;
}

//**************************************
//Takes the name of the object and a pointer to it
//creates components based on it's type and adds them to a vector
//which is returned.
std::vector<std::unique_ptr<Component>> ComponentAssetLibrary::search(std::string name, GameObject* owner)
//**************************************
{
	
	std::vector<std::unique_ptr<Component>> componentListPtrs;
	
	auto componentList{ library.find(name)->second };
	for( auto components: componentList)
	{
		switch (components)
		{
		case Components::body:
			componentListPtrs.push_back(std::make_unique<BodyComponent>(owner));
			break;
		case Components::health:
			componentListPtrs.push_back(std::make_unique<HealthComponent>(owner));
			break;
		case Components::renderer:
			componentListPtrs.push_back(std::make_unique<RendererComponent>(owner));
			break;
		case Components::userInput:
			componentListPtrs.push_back(std::make_unique<UserInputComponent>(owner));
			break;
		case Components::backpack:
			componentListPtrs.push_back(std::make_unique<BackpackComponent>(owner));
			break;
		case Components::inventory:
			componentListPtrs.push_back(std::make_unique<InventoryComponent>(owner));
			break;
		case Components::ghost:
			componentListPtrs.push_back(std::make_unique<GhostComponent>(owner));
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
bool ComponentAssetLibrary::addAsset(std::string name, std::vector<Components> componentList)
//**************************************
{
	library[name] = componentList;
	return true;
}