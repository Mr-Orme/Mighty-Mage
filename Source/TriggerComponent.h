#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Component.h"
#include "Initializers.h"

class GameObject;
class ResourceManager;
enum class Levels;
class TriggerComponent : public Component
{
public:
	//TODO:: switch notices to Trigger components with messages.
	enum class Type { exits, messages, battles };
	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	std::unique_ptr<Component> copyMe() const override;
	bool trigger(Direction direction);
private:
	Type name{ Type::exits };
	Direction direction{ Direction::N };
	Levels exitTo;
	std::string message;
};
///Trigger Component along with body component in Object.
//Collision detection with component triggers it.