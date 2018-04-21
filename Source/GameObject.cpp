#include "ComponentsList.h"

GameObject::GameObject()
{
	initialized = false;
}

GameObject::~GameObject()
{

}

//**************************************
//Get's the components up and running based on passed presests
bool GameObject::Initialize(GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	objectType = presets.objectType;
	joinedWith = NULL;
	////This sets the current object to the joinedObject in case a joint will be made.
	//presets.joinedObject = std::make_shared<GameObject>(shared_from_this());
	//the renderer component needs to be initialized first because the body component depends on it
	//It's initialization method has a check to see if it is already initialized 
	//so that when we go through all the components and initialize them in the next step, it won't do it again.
	
	std::shared_ptr<RendererComponent> component = GetComponent<RendererComponent>();
	if(component) component -> Initialize(presets);

	//initialize all components
	for ( auto comp :components)
	{
		
		comp -> Initialize(presets);
	}
	//start all components
	if(!initialized)
	{
		for (auto comp : components)
		{
			comp -> Start();
		}
		initialized = true;
	}
	return true;
}

//**************************************
//Adds the passed component to the object.
void GameObject::AddComponent(std::shared_ptr<Component> component)
//**************************************
{
	components.push_back(component);
}

//**************************************
//runs the update method for all attached components
std::shared_ptr<GameObject> GameObject::Update()
//**************************************

{
	std::shared_ptr<GameObject> newObject = NULL;
	for (auto comp : components)
	{
		std::shared_ptr<GameObject> tempObject = NULL;
		tempObject = comp -> Update();
		if(tempObject != NULL)
		{
			newObject = tempObject;
		}
			
	}
	return newObject;
}
//**************************************
//takes off a compoent and runs its finsih method
bool GameObject::removeComponents()
//**************************************
{
	for (auto comp : components)
	{
		comp -> Finish();
		
	}
	components.clear();
	return true;
}


