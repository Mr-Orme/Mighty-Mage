#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<memory>
#include<iostream>
#include<vector>
#include<tuple>
#include<optional>

#include "Definitions.h"
#include "Initializers.h"

#include "Component.h"
class GraphicsDevice;
class PhysicsDevice;
enum class Levels;
class GameObject
{
public:
	GameObject();
	~GameObject() {}

	enum class Type{player, wall, floor, door, extra};

	bool initialize(ObjectFactoryPresets& presets);
	void addComponent(std::unique_ptr<Component> component);
	
	std::tuple<std::unique_ptr<GameObject>, std::optional<Levels> > update(std::vector<std::unique_ptr<GameObject>>& objects);

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
	Direction direction{ Direction::N };
	GameObject* joinedWith{ nullptr };

};

#endif