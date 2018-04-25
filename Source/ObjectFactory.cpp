#include <memory>
#include "ObjectFactory.h"
#include "ComponentsList.h"
#include "View.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "ComponentAssetLibrary.h"
#include "GameObject.h"


//************************************************
//ALL ITEMS IN "presets" need to be set.
//based on the object type, it grabs components.
// and adds them to a newly created object.
//it initializes the object which also initializes all the components
GameObject* ObjectFactory::Create(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{

	//Create pointer to new objects
	GameObject* newObject = new GameObject();
	//Get list of components for the new object
	std::vector<Component*> componentList = presets.devices -> GetComponentLibrary() ->Search(presets.objectType, newObject);
	//Add each to the object
	for (auto comp : componentList)
	{
		newObject -> AddComponent(comp);
	}

	//the object can be walked over if this is not empty.
	if(!presets.gDirection.empty())
	{
		//add the ghost component
		GhostComponent* ghost = new GhostComponent(newObject);
		newObject -> AddComponent(ghost);
	}
		
	//Initialize the new object (this will also initialize components)
	if (newObject -> Initialize(presets))
	{
		//return the created and initialized object.
		return newObject;
	}
	else
	{
		delete newObject;
		std::cout << "Object did not initialize!" << std::endl;
		return nullptr;
	}
}

