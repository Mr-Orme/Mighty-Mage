#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Component.h"
#include "Definitions.h"

class GameObject;
class QuestComponent;
class ResourceManager;

class QuestCriteria: public Component
{
public:
	QuestCriteria() = delete;
	QuestCriteria(GameObject* owner, ResourceManager* devices);
	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	std::unique_ptr<Component> copyMe() const override;
	bool complete();
private:
	std::string successMessage;
	std::string failMessage;
	QuestComponent* quest{ nullptr };
	bool completeWithoutQuest{ false };
	
};