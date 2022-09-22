#ifndef HEALTH_H
#define HEALTH_H

#include "Component.h"
#include "Definitions.h"
class ResourceManager;

class HealthComponent : public Component
{
public:
	HealthComponent(GameObject* owner, ResourceManager* devices);
	~HealthComponent();

	std::unique_ptr<GameObject> update() override;
	bool initialize(ObjectFactoryPresets& presets) override;

	bool isDead() const {return isDead;}
	
	bool killMe(std::string deathSprite);
	bool killMe();
protected:
	int health{ 0 };
	bool isDead{ false };
	
};

#endif