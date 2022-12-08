#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Component.h"
#include "Definitions.h"

class GameObject;
class QuestCriteria;
class ResourceManager;
class QuestComponent : public Component
{
public:
	QuestComponent(GameObject* owner, ResourceManager* devices);
	QuestComponent() = default;
	~QuestComponent() = default;

	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	std::unique_ptr<Component> copyMe() const override;

private:
	enum class Type{kill, location, courier, retrieval, escape, trigger};
	int ID;
	Type name;

	std::string startMessage;
	std::string description;
	std::string completionMessage;

	std::vector<QuestCriteria*> notCompleted;
	std::vector<QuestCriteria*> completed;
	bool sequential{ false };
	/*How to generalize criteria for a quest?
	* Types:
	* 1. Get item(s)
	* 2. Defeat creature
	* 3. go somewhere.
	* 4. Solve riddle (would not need component)
	* 5. escape
	* 6. courier
	* 7. Trigger event
	* 
	* QuestCriteria class? When collide with something with this, search for matching quest component.
	* Each quest component would need a unique ID or list of QuestCritiria....
	* once notCompleted is empty, quest is done!
	* What about criteria that can be met before quest accepted?
	*	Perhaps a generic questComponent to store criteria.
	*   When new quest is picked up, check this one.
	* Ordered vs unordered quest criteria!
	*/

};

