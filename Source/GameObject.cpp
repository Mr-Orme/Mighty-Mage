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
bool GameObject::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	objectType = presets.objectType;
	joinedWith = NULL;
	////This sets the current object to the joinedObject in case a joint will be made.
	//presets.joinedObject = std::make_shared<GameObject>(shared_from_this());
	//the renderer component needs to be initialized first because the body component depends on it
	//It's initialization method has a check to see if it is already initialized 
	//so that when we go through all the components and initialize them in the next step, it won't do it again.
	
	RendererComponent* component = GetComponent<RendererComponent>();
	if(component) component -> Initialize(presets);

	//initialize all components
	std::vector<std::unique_ptr<Component>>::iterator compIter;
	for ( compIter= components.begin(); compIter!=components.end();compIter++)
	{
		
		(*compIter)-> Initialize(presets);
	}
	//start all components
	if(!initialized)
	{
		for (compIter = components.begin(); compIter != components.end(); compIter++)
		{
			(*compIter) -> Start();
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
	std::vector<std::unique_ptr<Component>>::iterator compIter;
	for (compIter = components.begin(); compIter != components.end(); compIter++)
	{
		if(GameObject* tempObject = (*compIter)->Update(); tempObject != nullptr)
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
	
	for (std::vector<std::unique_ptr<Component>>::iterator comp = components.begin();
		comp!=components.end(); 
		comp++)
	{
		(*comp) -> Finish();
		
	}
	components.clear();
	return true;
}


