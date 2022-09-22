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

	bool initialize(ObjectFactoryPresets& presets);

	std::unique_ptr<GameObject> update();

	float getAngle() const;
	Vector2D getPosition() const { return dPosition; }

protected:
	Vector2D dPosition{};
	Vector2D oldPosition{};
};

#endif