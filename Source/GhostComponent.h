#ifndef GHOST_H
#define GHOST_H

#include <map>
#include "Component.h"
#include "Definitions.h"

class GameObject;

class GhostComponent : public Component
{
public:
	GhostComponent(GameObject* owner, ResourceManager* devices);
	~GhostComponent();
	bool initialize(ObjectFactoryPresets& presets);

	bool canPass(Direction) const;
	void SetGhostDirection(Direction direction, bool isGhost){ghostDirection[direction]=isGhost;}
	void SetGhostDirection(std::map<Direction, bool> ghostDirection){this -> ghostDirection = ghostDirection;}

	
	std::unique_ptr<GameObject> update() override;
protected:
	std::map<Direction, bool> ghostDirection;
};

#endif