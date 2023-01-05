#pragma once
#include <variant>
#include "Component.h"

class StopUpdateDecorator :public Component
{
public:
	StopUpdateDecorator(GameObject* owner, ResourceManager* devices, Component* decorated, std::variant<GameObject*, int> condition);
	~StopUpdateDecorator();

	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	bool initialize(ObjectFactoryPresets& presets) override { return true; }
	std::unique_ptr<Component> copyMe() const override { return nullptr; }
	Component* getDecorated()const { return decorated.get(); }
	void remove();

private:
	
	bool conditionMet(GameObject* object);
	bool conditionMet(int & countdown);
	std::unique_ptr<Component> decorated{nullptr};
	std::variant<GameObject*, int> condition;


};