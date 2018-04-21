#ifndef GHOST_H
#define GHOST_H

#include <map>
#include "Component.h"
#include "Definitions.h"

class GameObject;

class GhostComponent : public Component
{
public:
	GhostComponent(std::shared_ptr<GameObject>);
	~GhostComponent();
	bool Initialize(GAME_OBJECTFACTORY_PRESETS& presets);

	std::map<GAME_DIRECTION, bool> getGhostDirection(){return ghostDirection;}
	void SetGhostDirection(GAME_DIRECTION direction, bool isGhost){ghostDirection[direction]=isGhost;}
	void SetGhostDirection(std::map<GAME_DIRECTION, bool> ghostDirection){this -> ghostDirection = ghostDirection;}

	void Start();
	std::shared_ptr<GameObject> Update();
	void Finish();
protected:
	std::map<GAME_DIRECTION, bool> ghostDirection;
};

#endif