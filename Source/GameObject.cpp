#include "ComponentsList.h"
#include "GameObject.h"
#include "Definitions.h"
#include "ResourceManager.h"
#include "Initializers.h"


//#include <sstream>
//#include "GraphicsDevice.h"
//#include "View.h"
GameObject::GameObject(){}


bool GameObject::initialize(ObjectFactoryPresets& presets)
{
	presets.owner = this;
	name = presets.id.type;
	
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
	return std::move(newObject);
}

bool GameObject::isA(Type name) const
{
	return this->name == name;
}

std::unique_ptr<Component> GameObject::swap(std::unique_ptr<Component> to, Component* from)
{
	std::unique_ptr<Component> toReturn{nullptr};
	if (auto found{ std::find_if(components.begin(), components.end(),[&](std::unique_ptr<Component>& current)
		{
			return from == current.get();
		}
	) }; found != components.end())
	{
		toReturn = std::move(*found);
		*found = std::move(to);
	}
return toReturn;
}




