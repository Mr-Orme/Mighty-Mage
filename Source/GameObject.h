#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>
#include<tuple>
#include<optional>

#include "Definitions.h"
#include "StopUpdateDecorator.h"
//#include "Initializers.h"

#include "Component.h"
struct ObjectFactoryPresets;
class GraphicsDevice;
class PhysicsDevice;
enum class Levels;
class GameObject
{
public:
	GameObject();
	~GameObject() {}

	enum class Type{player, wall, floor, door, extra, trigger};

	bool initialize(ObjectFactoryPresets& presets);
	void addComponent(std::unique_ptr<Component> component);
	
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects);

	void SetJoinedWith(GameObject* joinedWith){this -> joinedWith = joinedWith;}
	GameObject* getJoinedWith() const {return joinedWith;}

	bool isA(Type name) const;
	std::unique_ptr<Component> swap(std::unique_ptr<Component> to, Component* from);
	template<class T>
	T* getComponent()
	{
		for (auto& comp : components)
		{

			if (auto result{ dynamic_cast<T*>(comp.get()) }; result != nullptr)
			{
				return(result);
			}
			else if (
				auto result2{ dynamic_cast<StopUpdateDecorator*>(comp.get()) }; 
				result2 != nullptr
				)
			{
				if (auto decoratorResult{ dynamic_cast<T*>(result2->getDecorated()) }; decoratorResult != nullptr)
				{
					return decoratorResult;
				}
			}
		}

		return(nullptr);
	}

protected:
	std::vector<std::unique_ptr<Component>> components;

	Type name{ Type::extra };
	Direction direction{ Direction::N };
	GameObject* joinedWith{ nullptr };

};

#endif