
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H
#include <string>
#include "Vector2D.h"
#include "BodyComponent.h"
class GameObject;

class TiXmlElement;
class ResourceManager;
struct ObjectStats
{
	int health;
};
struct RotationStats
{
	float torque;
	float maxAngularVelocity;
	int radius;
	Vector2D center;
};

struct PhysicsStats
{
	enum class RotationDirection { left = -1, right = 1 };
	enum class BodyShape { rectangle, circle };
	enum class BodyType { staticBody, kinematic, dynamic };
	float spinSpeed;
	BodyType bodyType;
	BodyShape objectShape;
	float density;
	float friction;
	float restitution;
	float angularDamping;
	float linearDamping;
	float force;
	bool physicsOn;
};

class ObjectFactory
{
public:
	struct Presets
	{
		std::string objectType;
		Vector2D position;
		float angle;
		int health;//move to new AssetLibraryGame
		int destructTime;//move to new AssetLibraryGame
		ResourceManager* devices;
		std::map<BodyComponent::Direction, bool> gDirection;
		//std::unique_ptr<GameObject> joinedObject; //only needed for joint
	};

	std::unique_ptr<GameObject> Create(Presets& presets);
	protected:
	
};

#endif