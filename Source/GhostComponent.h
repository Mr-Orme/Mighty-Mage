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
	GhostComponent();
	~GhostComponent();
	
	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	std::unique_ptr<Component> copyMe() const override;
	//TODO::Can I eliminate canPass somehow??
	bool canPass(Direction) const;
	
protected:
	std::map<Direction, bool> ghostDirection;
};

#endif