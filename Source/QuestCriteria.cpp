#include "QuestCriteria.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Initializers.h"
QuestCriteria::QuestCriteria(GameObject* owner, ResourceManager* devices) :
	Component(owner, devices)
{
}

bool QuestCriteria::initialize(ObjectFactoryPresets& presets)
{
	if (!Component::initialize(presets))
	{
		std::cout << "Owner not present for Quest component";
		return false;
	}
	successMessage = presets.criteriaInitializers.successMessage;
	failMessage = presets.criteriaInitializers.failMessage;
	owningQuest = presets.criteriaInitializers.quest; 
	//can the criteria be satisfied, even if the quest has yet to be gotten.
	completeWithoutQuest = presets.criteriaInitializers.completeWithoutQuest;
	return true;
}

std::unique_ptr<GameObject> QuestCriteria::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	return std::unique_ptr<GameObject>();
}

std::unique_ptr<Component> QuestCriteria::copyMe() const
{
	return std::move(std::make_unique<QuestCriteria>(*this));
}

bool QuestCriteria::complete()
{
	if(!completeWithoutQuest)
		return false;
	return true;
}
