#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "Definitions.h"
#include "ObjectFactory.h"

class Component;
class GraphicsDevice;
class PhysicsDevice;

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
		for(auto comp : components)
		{
			T* target;
			if((target = std::dynamic_pointer_cast<T>(comp)))
			{
				return(target);
			}
		}
	
		//Return NULL;
		return(nullptr);
	}
	
	GameObject* Update();
	void draw();

	void SetJoinedWith(std::shared_ptr<GameObject> joinedWith){this -> joinedWith = joinedWith;}

	std::string getObjectType(){return objectType;}
	std::shared_ptr<GameObject> GetJoinedWith(){return joinedWith;}

	bool removeComponents();

protected:
	
	std::vector<std::unique_ptr<Component>> components;
    bool initialized;
	std::string objectType;
	std::shared_ptr<GameObject> joinedWith;

};

#endif