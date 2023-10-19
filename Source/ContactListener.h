#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H
#include <tuple>
#include "box2d.h"
#include "Definitions.h"
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

	
private:
	std::tuple<GameObject*, GameObject*> getObjects(b2Contact* contact);
	Direction travelDirection(GameObject* player) const;
	PhysicsDevice* pDevice{ nullptr };
	void pickUpItem(GameObject* player, GameObject* item);
};

#endif