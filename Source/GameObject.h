#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>
//#include "SDL.h"
//#include "SDL_ttf.h"
#include "Definitions.h"
#include "ObjectFactory.h"
//#include "ComponentsList.h"
class Component;
class GraphicsDevice;
class PhysicsDevice;
class BackpackComponent;
class BodyComponent;
class GhostComponent;
class HealthComponent;
class InventoryComponent;
class RendererComponent;
class UserInputComponent;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	~GameObject();
	
	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);
	void AddComponent(Component* component);

	template<class T>
	T* GetComponent()
	{

		for (std::vector<std::unique_ptr<Component>>::iterator compIter = components.begin(); compIter != components.end(); compIter++)
		{
			T* target = nullptr;
			if (target = dynamic_cast<T*>(compIter->get()))
			{
				return(target);
			}
		}

		//Return NULL;
		return(nullptr);
	}
	
	
	GameObject* Update();
	void draw();

	void SetJoinedWith(GameObject* joinedWith){this -> joinedWith = joinedWith;}

	std::string getObjectType(){return objectType;}
	GameObject* GetJoinedWith(){return joinedWith;}

	bool removeComponents();

protected:
	
	std::vector<std::unique_ptr<Component>> components;
    bool initialized;
	std::string objectType;
	GameObject* joinedWith;

};

#endif

