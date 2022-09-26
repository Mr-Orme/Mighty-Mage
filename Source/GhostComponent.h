#ifndef GHOST_H
#define GHOST_H

#include <map>
#include "Component.h"


class GameObject;

class GhostComponent : public Component
{
public:
	GhostComponent(GameObject* owner, ResourceManager* devices);
	~GhostComponent();
	
	bool initialize(ObjectFactory::Presets& presets) override;
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	//TODO::Can I eliminate canPass somehow??
	bool canPass(BodyComponent::Direction) const;
	//TODO:: remove if not needed
	/*void addGhostDirection(Direction direction, bool isGhost)
		{ghostDirection[direction]=isGhost;}
	void SetGhostDirection(std::map<Direction, bool> ghostDirection)
		{this -> ghostDirection = ghostDirection;}*/

	
	
protected:
	std::map<BodyComponent::Direction, bool> ghostDirection;
};

#endif