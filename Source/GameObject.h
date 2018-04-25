#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>

#include "Definitions.h"
#include "ObjectFactory.h"

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

class GameObject
{
public:
	GameObject();
	~GameObject();
	
	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);
	void AddComponent(Component* component);

	template<class T>
	T* GetComponent()
	{
		for (auto& component : components)
		{
			
			if (T* target = nullptr; target = dynamic_cast<T*>(component.get()))
			{
				return(target);
			}
		}
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

