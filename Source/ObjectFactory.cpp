#include "ObjectFactory.h"
#include "ComponentsList.h"
#include "View.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GameObject.h"
#include <memory>

//************************************************
//ALL ITEMS IN "presets" need to be set.
//based on the object type, it grabs components.
// and adds them to a newly created object.
//it initializes the object which also initializes all the components
std::unique_ptr<GameObject> ObjectFactory::Create(Presets& presets)
//**************************************
{

	//Create pointer to new objects
	std::unique_ptr<GameObject> newObject{ std::make_unique<GameObject>() };
	//Get list of components for the new object
	std::vector<std::unique_ptr<Component>> componentList
	{ 
		presets.devices->getComponentLibrary()->
		search(presets.objectType, newObject.get()) 
	};
	//Add each to the object
	for (auto& comp : componentList)
	{
		newObject -> addComponent(std::move(comp));
	}

	//the object can be walked over if this is not empty.
	if(!presets.gDirection.empty())
	{
		//add the ghost component
		newObject -> addComponent(std::make_unique<GhostComponent>(newObject.get(), presets.devices));
	}
		
	//Initialize the new object (this will also initialize components)
	if (newObject -> initialize(presets))
	{
		//return the created and initialized object.
		return newObject;
	}
	else
	{
		std::cout << "Object did not initialize!" << std::endl;
		return nullptr;
	}
}

