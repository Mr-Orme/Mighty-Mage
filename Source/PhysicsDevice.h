#ifndef PHYSICSDEVICE_H
#define PHYSICSDEVICE_H

#include "Definitions.h"
#include "Box2D.h"
#include "Texture.h"
#include "PhysicsAssetLibrary.h"
#include "GameObject.h"

class Object;

class PhysicsDevice{
public:
	PhysicsDevice(GAME_FLT gravityX, GAME_FLT gravityY);
	bool Initialize();
	bool Update(float dt);
	bool createFixture
		(
		GameObject* object,
		GAME_PHYSICS physics,
		GAME_OBJECTFACTORY_PRESETS presets
		);

	bool SetTransform(GameObject* object, GAME_VEC position, GAME_FLT angle);
	bool SetLinearVelocity(GameObject* object, GAME_VEC linearVelociy);
	bool SetAngularVelocity(GameObject* object, GAME_FLT angularVelocity);
	bool SetTorque(GameObject* object, GAME_FLT torque);
	bool SetLinearImpulse(GameObject* object, GAME_VEC forceVec, GAME_VEC forceCenter);
	bool SetStatic(GameObject* object);
	bool SetStopPhysics(GameObject* object);
	bool SetAngle(GameObject* object, GAME_FLT angle);

	GAME_FLT GetAngularVelocity(GameObject* object);
	GAME_VEC GetPosition(GameObject* object);
	GAME_FLT GetAngle(GameObject* object);
	GAME_VEC GetVelocity(GameObject* object);
	GAME_VEC GetLinearVelocity(GameObject* object);
	b2World* getWorld(){ return world; }
	
	bool CreatRevolvingJoint(GameObject* object1, GameObject* object2, GAME_VEC anchor1, GAME_VEC anchor2);

	
	b2Body* FindBody(GameObject* Object);
	b2Vec2 GV2PV(GAME_VEC gameVec);
	GAME_VEC PV2GV(b2Vec2 physicsVec);
	bool RemoveObject(GameObject* object);
	bool DestroyJoint(GameObject* object);
		
	b2World* world;
	
	inline float PW2RW(GAME_FLT x){return x*fPRV;}
	inline float RW2PW(GAME_FLT x){return x/fPRV;}
	inline float RW2PW(GAME_INT x){return (float)x/fPRV;}
	inline float RW2PWAngle(GAME_FLT x){return((float)x*(2.0f*3.14159f)/360.0f);} //degrees to radians
	inline float PW2RWAngle(GAME_FLT x){return((float)x*360.0f/(2.0f*3.14159f));} //radians to degrees
private:
	bool DestroyJoint(b2Body* body);
	const b2Vec2 gravity;
	GAME_VEC AlignCenters(GameObject* object);

	

	

};
#endif