#include <string>
#include "ContactListener.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ComponentsList.h"
#include "Definitions.h"
#include "SoundDevice.h"

Direction travelDirection(b2Body* body)
{
	auto lv{ body->GetLinearVelocity() };
	if (lv.x > 0) return Direction::E;
	else if (lv.x < 0) return Direction::W;
	else if (lv.y > 0) return Direction::S;
	else if (lv.y < 0) return Direction::N;
	return reinterpret_cast<GameObject*>(body->GetUserData().pointer)->getComponent<BodyComponent>()->getDirection();

}
void wallCollision(BodyComponent* playerBody, BodyComponent* wallBody, Direction travel)
{
	bool playWallSound{ false };
	Vector2D playerSquare{ playerBody->currentSquare() };
	Vector2D wallSquare{ wallBody->currentSquare() };
	

	switch (travel)
	{
	case Direction::N:
	case Direction::S:
		//HACK::because of how the level is built, the left and bottom walls are in the wrong square.
		//this adjusts for that. Same in Direction::E
		if (playerBody->getPosition().x > wallBody->getPosition().x + wallBody->getDimenions().x)
		{
			wallSquare.x--;
		}
		playWallSound = playerSquare.x == wallSquare.x;
		break;
	case Direction::W:
	case Direction::E:
		if (playerBody->getPosition().y >= wallBody->getPosition().y + wallBody->getDimenions().y)
		{
			wallSquare++;
		}
		playWallSound = playerSquare.y == wallSquare.y;
		break;
	default:
		break;
	}
	if (playWallSound)
		playerBody->getDevices()->getSoundDevice()->PlaySound(SoundEffect::Event::hitWall, 0, 2);
}
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	
	GameObject* objectA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
	GameObject* objectB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);

	if(objectA->isA(GameObject::Type::player))
	{
		if(objectB -> getComponent<InventoryComponent>())
		{
			PickUpItem(objectA, objectB);
		}
	}
	
	//walls always put the player as object B
	else if (objectB->isA(GameObject::Type::player))
	{
		
		if (auto ghost{ objectA->getComponent<GhostComponent>() };
			ghost != nullptr && ghost->canPass(travelDirection(bodyB))
			)
		{
			contact->SetEnabled(false);
			return;
		}

		if (objectA->isA(GameObject::Type::wall))
		{
			
			wallCollision(
				objectB->getComponent<BodyComponent>(), 
				objectA->getComponent<BodyComponent>(), 
				travelDirection(bodyB)
			);
		}
		
		

	}

}
void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}


void ContactListener::PickUpItem(GameObject* player, GameObject* item)
{

	ResourceManager* devices = player -> getComponent<BodyComponent>() -> getDevices();

	if(player -> getComponent<BackpackComponent>() -> pickUpItem(item))
	{
		devices -> getSoundDevice() -> PlaySound(SoundEffect::Event::pickup,0,3);
	}

}
