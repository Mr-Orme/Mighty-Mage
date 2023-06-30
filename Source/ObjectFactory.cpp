#include "ObjectFactory.h"
#include "ComponentsList.h"
#include "View.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GameObject.h"
#include "Initializers.h"
#include "ObjectLibrary.h"
#include <memory>
#include <iostream>

//************************************************
//ALL ITEMS IN "presets" need to be set.
//based on the object type, it grabs components.
// and adds them to a newly created object.
//it initializes the object which also initializes all the components
std::unique_ptr<GameObject> ObjectFactory::Create(ObjectFactoryPresets& presets)
//**************************************
{

	//Create pointer to new objects
	std::unique_ptr<GameObject> newObject{ std::make_unique<GameObject>() };
	ObjectDefinition& definition{ presets.devices->getObjectLibrary()->search(presets.objectType) };
	presets.bodyInitializers.physics = definition.physics;

	//Add each to the object
	for (const auto& component : definition.components)
	{
		newObject -> addComponent(component->copyMe());
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

