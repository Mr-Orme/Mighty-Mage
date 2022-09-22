#ifndef BODY_H
#define BODY_H

#include <memory>

#include "Component.h"
#include "Definitions.h"

class ResourceManager;
class GameObject;

class BodyComponent : public Component
{
public:
	BodyComponent(GameObject* owner, ResourceManager* devices);
	~BodyComponent();

	bool initialize(ObjectFactoryPresets& presets) override;

	std::unique_ptr<GameObject> update() override;

	float getAngle() const;
	Direction getDirection() const;
	Vector2D getPosition() const { return dPosition; }

protected:
	Vector2D dPosition{};
	Vector2D oldPosition{};
};

#endif