#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "Definitions.h"

class Component;
class GraphicsDevice;
class PhysicsDevice;

class GameObject
{
public:
	GameObject();
	~GameObject();
	
	bool initialize(ObjectFactoryPresets& presets);
	void addComponent(std::unique_ptr<Component> component);

	template<class T>
	std::unique_ptr<T>& GetComponent()
	{
		for(auto& comp : components)
		{
			std::unique_ptr<T> target;
			if((target = std::dynamic_pointer_cast<T>(comp)))
			{
				return(target);
			}
		}
	
		//Return nullptr;
		return(nullptr);
	}
	
	std::unique_ptr<GameObject> update();

	void SetJoinedWith(GameObject* joinedWith){this -> joinedWith = joinedWith;}

	std::string getObjectType(){return objectType;}
	GameObject* getJoinedWith(){return joinedWith;}
	

protected:
	
	std::vector<std::unique_ptr<Component>> components;
	std::string objectType;
	GameObject* joinedWith{ nullptr };

};

#endif