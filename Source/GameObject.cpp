#include "ComponentsList.h"
#include "GameObject.h"
#include "Definitions.h"

GameObject::GameObject(){}


//**************************************
//Get's the components up and running based on passed presests
	//the renderer component needs to be initialized first because the body component depends on it
	//It's initialization method has a check to see if it is already initialized 
	//so that when we go through all the components and initialize them in the next step, it won't do it again.
bool GameObject::initialize(ObjectFactoryPresets& presets)
//**************************************
{
	presets.owner = this;
	setType(presets.objectType);
	
	if (
		SpriteComponent* component{ getComponent<SpriteComponent>() };
		component != nullptr
		)
	{
		component->initialize(presets);
	}

	//initialize all components
	for ( auto& comp :components)
	{
		
		comp -> initialize(presets);
	}
	
	return true;
}

//**************************************
//Adds the passed component to the object.
void GameObject::addComponent(std::unique_ptr<Component> component)
//**************************************
{
	components.push_back(std::move(component));
}

//**************************************
//runs the update method for all attached components
std::unique_ptr<GameObject> GameObject::update(std::vector<std::unique_ptr<GameObject>>& objects)
//**************************************

{
	std::unique_ptr<GameObject> newObject{ nullptr };
	for (auto& comp : components)
	{
		std::unique_ptr<GameObject> tempObject{ comp->update(objects) };
		if(tempObject != nullptr)
		{
			newObject = std::move(tempObject);
		}
			
	}
	return newObject;
}

bool GameObject::isA(Type name) const
{
	return this->name == name;
}


void GameObject::setType(std::string sName)
{
	if (sName == "HWall" || sName =="HGhostWall")
	{
		name = Type::wall;
		direction = Direction::E;
	}
	else if (sName == "VWall" || sName == "VGhostWall" || sName == "TopFill")
	{
		name = Type::wall;
		direction = Direction::N;
	}
	else if (sName == "WallFloor" || sName == "GhostWallFloor")
	{
		name = Type::floor;
		direction = Direction::N;
		
	}
	else if (sName == "HDoor")
	{
		name = Type::door;
		direction = Direction::E;

	}
	else if (sName == "VDoor")
	{
		name = Type::door;
		direction = Direction::N;

	}
	else if (sName == "Player")
	{
		name = Type::player;
		direction = Direction::N;

	}
	else
	{
		name = Type::extra;
		direction = Direction::N;

	}
}



