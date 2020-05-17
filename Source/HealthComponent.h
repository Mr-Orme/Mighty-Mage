#ifndef HEALTH_H
#define HEALTH_H

#include "Component.h"
#include "Definitions.h"
class ResourceManager;

class HealthComponent : public Component
{
public:
	HealthComponent(std::shared_ptr<GameObject>);
	~HealthComponent();

	bool Initialize(GAME_OBJECTFACTORY_PRESETS& presets);

	GAME_INT GetHealth(){return health;}
	bool GetIsDead(){return isDead;}

	void SetHealth(GAME_INT health){this -> health = health;}
	void SetIsDead(bool isDead){this -> isDead = isDead;}


	bool KillObject(std::string deathSprite);
	bool KillObject();
	
	void Start();
	std::shared_ptr<GameObject> Update();
	void Finish();
protected:
	GAME_INT health;
	bool isDead;
	std::shared_ptr<ResourceManager> devices;
};

#endif