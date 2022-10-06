#ifndef INITIALIZERS_H
#define INITIALIZERS_H
#include <memory>
#include <string>
#include <map>
#include "Definitions.h"
#include "Vector2D.h"
#include "InputDevice.h"

//class ICallBack;
//class BodyCallBack;
class ResourceManager;
class BodyComponent;
class SpriteComponent;
class Texture;
enum class Levels;
struct ObjectStats
{
	int health;
};
struct SpritePresets
{
	bool createSprite{ false };
};
struct RotateBody
{
	float torque{ 0.0f };
	float maxAngularVelocity{ 0.0f };
	int radius{ 0 };
	Vector2D center{ 0, 0 };
};

enum class BodyShape { Rectangle, Circle };
enum class BodyType { Static, Kinematic, Dynamic };
enum class JointType { Revolute, Distance, Prismatic, Wheel, Weld, Pulley, Friction, Gear, Mouse, Rope, None };
struct PhysicsStats
{
	float spinSpeed{ 0.0f };
	BodyType bodyType{ BodyType::Static };
	BodyShape bodyShape{ BodyShape::Rectangle };
	float density{ 0.0f };
	float friction{ 0.0f };
	float restitution{ 0.0f };
	float angularDamping{ 0.0f };
	float linearDamping{ 0.0f };
	float force{ 0.0f };

	bool physicsOn{ false };
};
struct BodyPresets
{
	BodyPresets() {}
	~BodyPresets() {}
	bool createBody{ false };
	Vector2D position{ 0,0 };
	Vector2D dimensions{ 0,0 };
	float angle{ 0.0f };
	PhysicsStats physics;
	SpriteComponent* sprite{ nullptr };
	//std::vector<std::unique_ptr<BodyCallBack>> callBacks;
};

struct SecondayJoint
{
	BodyPresets bodyInitializers;
	SpritePresets spriteInitializers;
	BodyComponent* BodyB{ nullptr };
	Vector2D AnchorA{ 0, 0 };
	Vector2D AnchorB{ 0, 0 };
	float referenceAngle{ 0.0f };
	float JointLimit{ 0.0f };
	std::unique_ptr<SecondayJoint> jointPresets{ nullptr };

};
struct PrimaryJoint
{
	bool createJoint{ false };
	JointType type{ JointType::None };
	bool CollideConnected{ false };
	BodyComponent* BodyA{ nullptr };
	BodyPresets bodyInitializers;
	SpritePresets spriteInitializers;
	SecondayJoint joinedTo;
};


struct UserInputPresets
{
	bool createUserInput{ false };
	InputDevice::Inputs TriggeredInput;
	int triggeringEvent;
	//ICallBack* callback;

};
struct SteeringPresets
{
	enum class TargetType { setVector, mouse };
	bool createSteering{ false };
	bool seek{ false };
	bool arrive{ false };
	TargetType type{ TargetType::mouse };
	Vector2D staticTargetVector;
	
};
struct TriggerPresets
{
	int name{}; //convert to TriggerComponent::Type
	Direction direction{ Direction::N };
	Levels exitTo;
	std::string message;
};
struct ObjectFactoryPresets
{
	std::string objectType{ "" };
	SpritePresets spriteInitializers;
	BodyPresets bodyInitializers;
	UserInputPresets userInputInitializers;
	SteeringPresets steeringInitializers;
	TriggerPresets triggerInitializers;
	PrimaryJoint jointInitializers;
	std::map<Direction, bool> gDirection;
	//int health{ 0 };//move to new AssetLibraryGame
	ResourceManager* devices{ nullptr };
	GameObject* owner{ nullptr };
};
#endif // !INITIALIZERS_H

