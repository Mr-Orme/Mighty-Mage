#ifndef BODY_H
#define BODY_H

#include <memory>

#include "Component.h"

#include "Vector2D.h"
#include "Definitions.h"


class ResourceManager;
class GameObject;

class BodyComponent : public Component
{
public:
	

	BodyComponent(GameObject* owner, ResourceManager* devices);
	BodyComponent();
	~BodyComponent();

	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	std::unique_ptr<Component> copyMe() const override;
	
	void turnLeft();
	void turnRight();
	void moveForward();
	void moveBackward();

	void stop();
	float getAngle() const;
	Direction getDirection() const;
	Vector2D getPosition() const;
	void turnOffPhysics() const;
	Vector2D currentSquare() const;
	Vector2D getDimenions() const;
protected:
	enum Way { forward = -1, backward = 1 };
	void linearMovement(Way direction);
	
	const int baseForceMultiplier{ 500 }; //How fast does the player move.
	int forceMultiplier{ baseForceMultiplier };
	const int runMultiplier{ 2 }; //How many times faster is running then walking
	const int angleAdjust{ 90 }; //How many degrees does the player turn each time
	Vector2D dimensions{ 0,0 };
	
};

#endif