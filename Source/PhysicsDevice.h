#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H


#include "ContactListener.h"
#include "Box2D.h"

class GameObject;
class Box2DDebugDraw;
class ResourceManager;


class PhysicsDevice{
public:
	//TODO::make Vector2D
	PhysicsDevice(float gravityX, float gravityY, ResourceManager* devices);

	bool Update(float dt);

	void debugDraw();

	bool createFixture
		(
		GameObject* object,
		PhysicsStats physics,
			ObjectFactory::Presets presets
		);

	bool SetTransform(GameObject* object, Vector2D position, float angle);
	bool SetLinearVelocity(GameObject* object, Vector2D linearVelociy);
	bool SetAngularVelocity(GameObject* object, float angularVelocity);
	bool SetTorque(GameObject* object, float torque);
	bool SetLinearImpulse(GameObject* object, Vector2D forceVec, Vector2D forceCenter);
	bool SetStatic(GameObject* object);
	bool SetStopPhysics(GameObject* object);
	bool SetAngle(GameObject* object, float angle);
	

	float GetAngularVelocity(GameObject* object);
	Vector2D GetPosition(GameObject* object);
	float getAngle(GameObject* object);
	Vector2D GetVelocity(GameObject* object);
	Vector2D GetLinearVelocity(GameObject* object);
	
	
	bool CreatRevolvingJoint(GameObject* object1, GameObject* object2, Vector2D anchor1, Vector2D anchor2);

	
	b2Body* FindBody(GameObject* Object);
	b2Vec2 GV2PV(Vector2D gameVec);
	Vector2D PV2GV(b2Vec2 physicsVec);
	bool RemoveObject(GameObject* object);
	bool DestroyJoint(GameObject* object);
		
	
	
	inline float PW2RW(float x) { return x * physicsModifier; }
private:
	const float physicsModifier = 10.0f;
	inline float RW2PW(float x) { return x / physicsModifier; }
	inline float RW2PW(int x) { return (float)x / physicsModifier; }
	inline float RW2PWAngle(float x) { return((float)x * (2.0f * 3.14159f) / 360.0f); } //degrees to radians
	inline float PW2RWAngle(float x) { return((float)x * 360.0f / (2.0f * 3.14159f)); } //radians to degrees
	bool DestroyJoint(b2Body* body);
	Vector2D AlignCenters(GameObject* object);
	
	std::unique_ptr<b2World> world{ nullptr };
	std::unique_ptr<Box2DDebugDraw> debugDrawer{ nullptr };
	std::unique_ptr<ContactListener> listner{ nullptr };
	const b2Vec2 gravity;
	

	

	

};
#endif