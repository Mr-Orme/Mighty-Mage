#ifndef HEALTH_H
#define HEALTH_H

#include "Component.h"

class ResourceManager;

class HealthComponent : public Component
{
public:
	HealthComponent(GameObject* owner, ResourceManager* devices);
	~HealthComponent();

	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	bool initialize(ObjectFactory::Presets& presets) override;

	bool isDead() const {return dead;}
	
	bool killMe(std::string deathSprite);
	bool killMe();
protected:
	int health{ 0 };
	bool dead{ false };
	
};

#endif