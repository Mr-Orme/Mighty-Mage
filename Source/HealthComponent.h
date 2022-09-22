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

	bool initialize(ObjectFactoryPresets& presets);

	int GetHealth(){return health;}
	bool GetIsDead(){return isDead;}

	void SetHealth(int health){this -> health = health;}
	void SetIsDead(bool isDead){this -> isDead = isDead;}


	bool KillObject(std::string deathSprite);
	bool KillObject();
	
	void Start();
	std::unique_ptr<GameObject> update();
	void Finish();
protected:
	int health{ 0 };
	bool isDead{ false };
	
};

#endif