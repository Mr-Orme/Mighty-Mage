
#include "GameObject.h"
#include "Definitions.h"
#include "ResourceManager.h"

//TODO::Get rid of this
#include <sstream>


GameObject::GameObject(){}


bool GameObject::initialize(ObjectFactoryPresets& presets)
{
	presets.owner = this;
	setType(presets.objectType);
	
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
	//if (name == Type::wall)
	//{
	//	auto center{ getComponent<BodyComponent>()->getCenter() };
	//	center = getComponent<BodyComponent>()->getDevices()->pixel2Square(center);
	//	std::stringstream output;
	//	output << center.x << ", " << center.y;
	//	auto gDevice{ getComponent<BodyComponent>()->getDevices()->getGraphicsDevice() };
	//	gDevice->text2Screen(output.str(), gDevice->getView()->relativePosition(center));
	//}

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
	else if (sName == "Trigger")
	{
		name = Type::trigger;
		direction = Direction::N;
	}
	else
	{
		name = Type::extra;
		direction = Direction::N;

	}
}



