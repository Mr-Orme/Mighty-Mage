#include <string>
#include "ContactListener.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ComponentsList.h"
#include "Definitions.h"
#include "SoundDevice.h"
#include "GraphicsDevice.h" //TODO::get rid of this
#include <sstream>

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
	Vector2D playerSquare{ playerBody->getDevices()->pixel2Square(playerBody->getCenter()) };
	Vector2D wallSquare{ wallBody->getDevices()->pixel2Square(wallBody->getCenter()) };
	/*std::cout << "Player Square: " << playerSquare.x << ", " << playerSquare.y << std::endl;
	std::cout << "wall Square: " << wallSquare.x << ", " << wallSquare.y << std::endl << std::endl;*/
	
	
	switch (travel)
	{
	case Direction::N:
	case Direction::S:
		//HACK::because of how the level is built, the left and bottom walls are in the wrong square.
		//this adjusts for that. Same in Direction::E
		if (playerBody->getPosition().x > wallBody->getPosition().x + wallBody->getDimensions().x)
		{
			wallSquare.x--;
		}
		playWallSound = playerSquare.x == wallSquare.x;
		break;
	case Direction::W:
	case Direction::E:
		if (playerBody->getPosition().y >= wallBody->getPosition().y + wallBody->getDimensions().y)
		{
			wallSquare.y++;
		}
		playWallSound = playerSquare.y == wallSquare.y;
		break;
	default:
		break;
	}
	if (playWallSound)
		playerBody->getDevices()->getSoundDevice()->PlaySound(SoundEffect::Event::hitWall, 0, 2);
}

void hitTrigger(GameObject* trigger, GameObject* player, b2Contact* contact, Direction travel)
{
	//player->getComponent<BodyComponent>()->getDevices()->getGraphicsDevice()->
	//	text2Screen(
	//		std::to_string((int)player->getComponent<BodyComponent>()->getDirection()), { 10,10 });

	if (player->getComponent<BodyComponent>()->getDirection() == travel && !trigger->getComponent<TriggerComponent>()->trigger
	(
		player->getComponent<BodyComponent>()->getDirection(), player
	))
	{
		
	}
	contact->SetEnabled(false);
}
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	
	GameObject* objectA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
	GameObject* objectB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);
	
	if (objectA->isA(GameObject::Type::trigger))
	{
		hitTrigger(objectA, objectB, contact, travelDirection(bodyB));
	}
	else if (objectB->isA(GameObject::Type::trigger))
	{
		hitTrigger(objectB, objectA, contact, travelDirection(bodyA));
	}

	else if(objectA->isA(GameObject::Type::player))
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
