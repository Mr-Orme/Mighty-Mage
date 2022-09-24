#include <string>
#include "ContactListener.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ComponentsList.h"

Direction travelDirection(b2Body* body)
{
	auto object{ reinterpret_cast<GameObject*>(body->GetUserData().pointer) };
	if (body->GetLinearVelocity().y <= 0)
	{

		return object->getComponent<BodyComponent>()->getDirection();
	}
	else if (body->GetLinearVelocity().y >= 0)
	{
		//going backwards.
		return (Direction)(((int)object->getComponent<BodyComponent>()->getDirection() + 180) % 360);
		
	}

	return Direction::N;
}
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
			bool playWallSound{ false };
			BodyComponent* playerBody{ objectB->getComponent<BodyComponent>() };
			BodyComponent* wallBody{ objectA->getComponent<BodyComponent>() };
			Vector2D playerSquare{ playerBody->currentSquare() };
			Vector2D wallSquare{ wallBody->currentSquare() };
			//adjust wall sqaure if player.x > wall.x or player.y < wall.y
	
			switch (travelDirection(bodyB))
			{
			case Direction::N:
			case Direction::S:
				if (playerBody->getPosition().x > wallBody->getPosition().x)
				{
					wallSquare++;
				}
				playWallSound = playerSquare.x == wallSquare.x;
				break;
			case Direction::W:
			case Direction::E:
				if (playerBody->getPosition().y > wallBody->getPosition().y)
				{
					wallSquare++;
				}
				playWallSound = playerSquare.y == wallSquare.y;
				break;
			default:
				break;
			}
			if(playWallSound)
				playerBody->getDevices()->GetSoundDevice()->PlaySound("wall", 0, 2);
		}
		
		if (auto ghost{ objectA->getComponent<GhostComponent>() };
			ghost != nullptr && ghost->canPass(travelDirection(bodyB))
			)
		{
			contact->SetEnabled(false);
		}

	}

}
void ContactListener::PostSolve(b2Contact*, const b2ContactImpulse*)
{
}


void ContactListener::PickUpItem(GameObject* player, GameObject* item)
{

	ResourceManager* devices = player -> getComponent<BodyComponent>() -> getDevices();

	if(player -> getComponent<BackpackComponent>() -> pickUpItem(item))
	{
		devices -> GetSoundDevice() -> PlaySound("found",0,3);
	}

}