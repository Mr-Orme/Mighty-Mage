#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>
#include "Definitions.h"

class Component;
class GraphicsDevice;
class PhysicsDevice;

class GameObject
{
public:
	GameObject();
	~GameObject();
	enum class Type{player, wall, floor, door, extra};
	bool initialize(ObjectFactoryPresets& presets);
	void addComponent(std::unique_ptr<Component> component);

	template<class T>
	T* getComponent()
	{
		for(auto& comp : components)
		{
			
			if (auto result{ dynamic_cast<T*>(comp.get()) }; result !=nullptr)
			{
				return(result);
			}
		}
	
		//Return nullptr;
		return(nullptr);
	}
	
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects);

	void SetJoinedWith(GameObject* joinedWith){this -> joinedWith = joinedWith;}

	bool isWall() const;
	bool isPlayer() const;
	bool isDoor() const;
	GameObject* getJoinedWith(){return joinedWith;}
	

protected:
	void setType(std::string);

	std::vector<std::unique_ptr<Component>> components;

	Type name{ Type::floor };
	Direction direction{ Direction::N };
	GameObject* joinedWith{ nullptr };

};

#endif