#include "ComponentsList.h"

GameObject::GameObject(){}

GameObject::~GameObject()
{

}

//**************************************
//Get's the components up and running based on passed presests
bool GameObject::initialize(ObjectFactoryPresets& presets)
//**************************************
{
	objectType = presets.objectType;
	joinedWith = nullptr;
	////This sets the current object to the joinedObject in case a joint will be made.
	//presets.joinedObject = std::make_unique<GameObject>(shared_from_this());
	//the renderer component needs to be initialized first because the body component depends on it
	//It's initialization method has a check to see if it is already initialized 
	//so that when we go through all the components and initialize them in the next step, it won't do it again.
	
	std::unique_ptr<RendererComponent>& component = GetComponent<RendererComponent>();
	if(component) component -> initialize(presets);

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
std::unique_ptr<GameObject> GameObject::update()
//**************************************

{
	std::unique_ptr<GameObject> newObject{ nullptr };
	for (auto& comp : components)
	{
		std::unique_ptr<GameObject> tempObject{ comp->update() };
		if(tempObject != nullptr)
		{
			newObject = std::move(tempObject);
		}
			
	}
	return newObject;
}



