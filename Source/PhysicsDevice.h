#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H
#include<unordered_map>

#include "ContactListener.h"
#include "Box2D.h"
#include "Initializers.h"
class GameObject;
class Box2DDebugDraw;
class ResourceManager;


class PhysicsDevice{
public:

	PhysicsDevice(Vector2D gravity, ResourceManager* devices);
	~PhysicsDevice() = default;
	bool Update(float dt);

	void debugDraw();

	bool createFixture(GameObject* object, BodyPresets presets);

	
	bool setLinearVelocity(GameObject* object, Vector2D linearVelociy);
	
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
	GameObject* findObject(b2Body* body) const;
	b2Vec2 GV2PV(Vector2D gameVec);
	Vector2D PV2GV(b2Vec2 physicsVec);
	bool RemoveObject(GameObject* object);
	bool DestroyJoint(GameObject* object);
		
	
	
	inline float PW2RW(float x) { return x * physicsModifier; }
private:
	bool setTransform(GameObject* object, Vector2D position, float angle);
	bool setAngularVelocity(GameObject* object, float angularVelocity);

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
	std::unordered_map<GameObject*, b2Body*> bodyMap;

	

	

};
#endif