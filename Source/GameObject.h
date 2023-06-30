#ifndef MIGHTYMAGE_GAMEOBJECT_H
#define MIGHTYMAGE_GAMEOBJECT_H

#include<memory>
#include<vector>

#include "Component.h"
#include "Definitions.h"
#include "Initializers.h"


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

	void setJoinedWith(GameObject* joinedWith){this -> joinedWith = joinedWith;}
	GameObject* getJoinedWith() const {return joinedWith;}

	bool isA(Type name) const;

	template<class T>
	T* getComponent()
	{

		auto result{ std::find_if(components.begin(), components.end(), [](const auto& comp) {return dynamic_cast<T*>(comp.get()) != nullptr; }) };

		if (result != components.end())
		{
			return dynamic_cast<T*>((*result).get());
		}

		return nullptr;
		
	}

protected:
	void setType(std::string);

	std::vector<std::unique_ptr<Component>> components;

	Type name{ Type::extra };
	Direction direction{ Direction::N };
	GameObject* joinedWith{ nullptr };

};

#endif