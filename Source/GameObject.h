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

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	~GameObject();
	
	bool Initialize(GAME_OBJECTFACTORY_PRESETS& presets);
	void AddComponent(std::shared_ptr<Component> component);

	template<class T>
	std::shared_ptr<T> GetComponent()
	{
		for(auto comp : components)
		{
			std::shared_ptr<T> target;
			if((target = std::dynamic_pointer_cast<T>(comp)))
			{
				return(target);
			}
		}
	
		//Return NULL;
		return(std::shared_ptr<T>());
	}
	
	std::shared_ptr<GameObject> Update();

	void SetJoinedWith(std::shared_ptr<GameObject> joinedWith){this -> joinedWith = joinedWith;}

	std::string getObjectType(){return objectType;}
	std::shared_ptr<GameObject> GetJoinedWith(){return joinedWith;}

	bool removeComponents();

protected:
	
	std::vector<std::shared_ptr<Component>> components;
    bool initialized;
	std::string objectType;
	std::shared_ptr<GameObject> joinedWith;

};

#endif