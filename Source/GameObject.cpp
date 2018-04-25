#include "ComponentsList.h"
#include "GameObject.h"

GameObject::GameObject()
{
	initialized = false;
}

GameObject::~GameObject()
{

}

//**************************************
//Get's the components up and running based on passed presests
bool GameObject::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	objectType = presets.objectType;
	joinedWith = nullptr;
	
	//the renderer component needs to be initialized first because the body component depends on it
	//It's initialization method has a check to see if it is already initialized 
	//so that when we go through all the components and initialize them in the next step, it won't do it again.
	RendererComponent* component = GetComponent<RendererComponent>();
	if(component) component -> Initialize(presets);

	//initialize all components
	for ( auto& component : components)
	{
	
		
		component-> Initialize(presets);
	}
	//start all components
	if(!initialized)
	{
		for (auto& component : components)
		{
			component -> Start();
		}
		initialized = true;
	}
	return true;
}

//**************************************
//Adds the passed component to the object.
void GameObject::AddComponent(Component* component)
//**************************************
{
	components.push_back(std::unique_ptr<Component>(component));
}

//**************************************
//runs the update method for all attached components
GameObject* GameObject::Update()
//**************************************

{
	GameObject* newObject = nullptr;
	for (auto& component : components)
	{
		if(GameObject* tempObject = component ->Update(); tempObject != nullptr)
		{
			newObject = tempObject;
		}
			
	}
	return newObject;
}
void GameObject::draw()
{
	if (RendererComponent* sprite = GetComponent<RendererComponent>(); sprite != nullptr)
	{
		sprite->Draw();
	}
}
//**************************************
//takes off a compoent and runs its finsih method
bool GameObject::removeComponents()
//**************************************
{
	
	for (auto& comp: components)
	{
		comp -> Finish();
		
	}
	components.clear();
	return true;
}

