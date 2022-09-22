#include "ComponentAssetLibrary.h"
#include "ComponentsList.h"
#include "GameObject.h"
#include "ResourceManager.h"

ComponentAssetLibrary::ComponentAssetLibrary(ResourceManager* devices) :devices(devices)
{}


//**************************************
//Takes the name of the object and a pointer to it
//creates components based on it's type and adds them to a vector
//which is returned.
//TODO::This is a really bad name for this!
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
			componentListPtrs.emplace_back(std::make_unique<BodyComponent>(owner, devices));
			break;
		case Components::health:
			componentListPtrs.emplace_back(std::make_unique<HealthComponent>(owner, devices));
			break;
		case Components::renderer:
			componentListPtrs.emplace_back(std::make_unique<RendererComponent>(owner, devices));
			break;
		case Components::userInput:
			componentListPtrs.emplace_back(std::make_unique<UserInputComponent>(owner, devices));
			break;
		case Components::backpack:
			componentListPtrs.emplace_back(std::make_unique<BackpackComponent>(owner, devices));
			break;
		case Components::inventory:
			componentListPtrs.emplace_back(std::make_unique<InventoryComponent>(owner, devices));
			break;
		case Components::ghost:
			componentListPtrs.emplace_back(std::make_unique<GhostComponent>(owner, devices));
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