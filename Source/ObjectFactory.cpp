#include "ObjectFactory.h"
#include "ComponentsList.h"
#include "View.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GameObject.h"
#include "Initializers.h"
#include "ObjectLibrary.h"
#include <memory>

//************************************************
//ALL ITEMS IN "presets" need to be set.
//based on the object type, it grabs components.
// and adds them to a newly created object.
//it initializes the object which also initializes all the components
std::unique_ptr<GameObject> ObjectFactory::Create(ObjectFactoryPresets& presets, ResourceManager* devices)
//**************************************
{

	//Create pointer to new objects
	std::unique_ptr<GameObject> newObject{ std::make_unique<GameObject>() };
	ObjectDefinition& definition{ devices->getObjectLibrary()->search(presets.objectType) };
	presets.bodyInitializers.physics = definition.physics;

	for (const auto& component : definition.components)
	{
		newObject -> addComponent(component->copyMe());
	}

	if(!presets.gDirection.empty())
	{
		newObject -> addComponent(std::make_unique<GhostComponent>(newObject.get(), devices));
	}
		
	//Initialize the new object (this will also initialize components)
	//Owner and devices set here for each component.
	if (newObject -> initialize(presets))
	{
		return newObject;
	}
	else
	{
		std::cout << "Object did not initialize!" << std::endl;
		return nullptr;
	}
}

