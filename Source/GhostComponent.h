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
	
	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<GameObject> update() override;
	//TODO::Can I eliminate canPass somehow??
	bool canPass(Direction) const;
	//TODO:: remove if not needed
	/*void addGhostDirection(Direction direction, bool isGhost)
		{ghostDirection[direction]=isGhost;}
	void SetGhostDirection(std::map<Direction, bool> ghostDirection)
		{this -> ghostDirection = ghostDirection;}*/

	
	
protected:
	std::map<Direction, bool> ghostDirection;
};

#endif