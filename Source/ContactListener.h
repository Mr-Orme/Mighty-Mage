#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include "box2d.h"
class GameObject;
class PhysicsDevice;
class ContactListener : public b2ContactListener
{
public:
	ContactListener(PhysicsDevice* pDevice);
	~ContactListener() {}
	void BeginContact(b2Contact* contact){}

	void EndContact(b2Contact* contact){}

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);


	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void PickUpItem(GameObject* player, GameObject* item);
private:
	PhysicsDevice* pDevice{ nullptr };
};

#endif