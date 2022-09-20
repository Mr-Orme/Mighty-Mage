#ifndef GHOST_H
#define GHOST_H

#include <map>
#include "Component.h"
#include "Definitions.h"

class GameObject;

class GhostComponent : public Component
{
public:
	GhostComponent(std::unique_ptr<GameObject>);
	~GhostComponent();
	bool initialize(ObjectFactoryPresets& presets);

	std::map<Direction, bool> getGhostDirection(){return ghostDirection;}
	void SetGhostDirection(Direction direction, bool isGhost){ghostDirection[direction]=isGhost;}
	void SetGhostDirection(std::map<Direction, bool> ghostDirection){this -> ghostDirection = ghostDirection;}

	void Start();
	std::unique_ptr<GameObject> update();
	void Finish();
protected:
	std::map<Direction, bool> ghostDirection;
};

#endif