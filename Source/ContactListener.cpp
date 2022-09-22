#include <string>
#include "ContactListener.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ComponentsList.h"

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    //Grab the two Physics Bodies involved in the Contact
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	//Cast them to object pointers
	GameObject* objectA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
	GameObject* objectB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);

	//find their types
	
			
	if(objectA->isPlayer())
	{
		
		//if we found a pickupable item, grab it.
		if(objectB -> getComponent<InventoryComponent>())
		{
			
			PickUpItem(objectA, objectB);
		}
	}
	
	//walls always put the player as object B
	else if (objectB->isPlayer())
	{
		if (objectA->isWall())
		{
			objectB->getComponent<UserInputComponent>()->collidingWithWall(true);
		}
		else
		{
			objectB->getComponent<UserInputComponent>()->collidingWithWall(false);
		}
		
		if (auto ghost{ objectA->getComponent<GhostComponent>() };
			ghost != nullptr &&
			(bodyB->GetLinearVelocity().y <= 0 && ghost->canPass(Direction::N))
			|| (bodyB->GetLinearVelocity().y >= 0 && ghost->canPass(Direction::S))
			|| (bodyB->GetLinearVelocity().x >= 0 && ghost->canPass(Direction::E))
			|| (bodyB->GetLinearVelocity().x <= 0 && ghost->canPass(Direction::W))
			)
		{
			contact->SetEnabled(false);
		}

	}

}

void ContactListener::PickUpItem(GameObject* player, GameObject* item)
{
	//TODO::strip item and make new item to pass to backback, move all components. mark to delete item...
	// Maybe use event handler somehow?
	//grab the resource manager from the player's body component
	ResourceManager* devices = player -> getComponent<BodyComponent>() -> getDevices();
	//if there is space to add it to the backpack, play the "found item" sound. . .
	//if(player -> getComponent<BackpackComponent>() -> addItem(item))
	{
		devices -> GetSoundDevice() -> PlaySound("found",0,3);
	}

}