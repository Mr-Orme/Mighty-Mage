#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>
#include "ObjectFactory.h"
#include "BodyComponent.h"

class Component;
class GraphicsDevice;
class PhysicsDevice;

class GameObject
{
public:
	GameObject();
	~GameObject();

	enum class Type{player, wall, floor, door, extra};

	bool initialize(ObjectFactory::Presets& presets);
	void addComponent(std::unique_ptr<Component> component);
	
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects);

	void SetJoinedWith(GameObject* joinedWith){this -> joinedWith = joinedWith;}
	GameObject* getJoinedWith() const {return joinedWith;}

	bool isA(Type name) const;

	template<class T>
	T* getComponent()
	{
		for (auto& comp : components)
		{

			if (auto result{ dynamic_cast<T*>(comp.get()) }; result != nullptr)
			{
				return(result);
			}
		}

		return(nullptr);
	}

protected:
	void setType(std::string);

	std::vector<std::unique_ptr<Component>> components;

	Type name{ Type::extra };
	BodyComponent::Direction direction{ BodyComponent::Direction::N };
	GameObject* joinedWith{ nullptr };

};

#endif