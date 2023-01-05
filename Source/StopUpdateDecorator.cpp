#include "StopUpdateDecorator.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
StopUpdateDecorator::StopUpdateDecorator(GameObject* owner, ResourceManager* devices, Component* decorated, std::variant<GameObject*, int> condition)
	:Component(owner, devices), 
	decorated(owner->swap(std::unique_ptr<Component>(this), decorated)),
	condition(condition)
{}

StopUpdateDecorator::~StopUpdateDecorator()
{
	//remove();
}

std::unique_ptr<GameObject> StopUpdateDecorator::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	if (auto value{ std::get_if<int>(&condition) }; value)
	{
		if (*value > 0)
		{
			(*value)--;
		}
		else
		{
			remove();
		}
	}
	else if (auto value{ std::get_if<GameObject*>(&condition) }; value)
	{
		if ((*value)->getComponent<BodyComponent>()->getSqaure() != _owner->getComponent<BodyComponent>()->getSqaure())
		{
			remove();
		}
	}
	return nullptr;
}
void StopUpdateDecorator::remove()
{
	_owner->swap(std::move(decorated), this);
}