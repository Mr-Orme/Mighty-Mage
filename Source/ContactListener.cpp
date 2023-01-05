#include <string>
#include "ContactListener.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ComponentsList.h"
#include "Definitions.h"
#include "SoundDevice.h"
#include "GraphicsDevice.h" 
#include "StopUpdateDecorator.h"
//#include <sstream>

Direction travelDirection(b2Body* body)
{
	auto lv{ body->GetLinearVelocity() };
	if (lv.x > 0) return Direction::E;
	else if (lv.x < 0) return Direction::W;
	else if (lv.y > 0) return Direction::S;
	else if (lv.y < 0) return Direction::N;
	return reinterpret_cast<GameObject*>(body->GetUserData().pointer)->getComponent<BodyComponent>()->getDirection();

}
void wallCollision(BodyComponent* playerBody, BodyComponent* wallBody, Direction travel, ResourceManager* devices)
{
	bool playWallSound{ false };
	Vector2D playerSquare{devices->pixel2Square(playerBody->getCenter()) };
	Vector2D wallSquare{ devices->pixel2Square(wallBody->getCenter()) };
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
		devices->getSoundDevice()->PlaySound(SoundEffect::Event::hitWall, 0, 2);
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
ContactListener::ContactListener(ResourceManager* devices)
	:devices(*devices)
{
}
std::tuple<GameObject*, GameObject*> findPlayer_Other(b2Body* bodyA, b2Body* bodyB)
{
	auto objectA{ reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer) };
	auto objectB{ reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer) };
	GameObject* player{ nullptr };
	GameObject* other{ nullptr };
	if (objectA->isA(GameObject::Type::player))
	{
		player = objectA;
		other = objectB;
	}
	else if(objectB->isA(GameObject::Type::player))
	{
		player = objectB;
		other = objectA;
	}
	return { player, other };

}
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	auto bodyA{ contact->GetFixtureA()->GetBody() };
	auto bodyB{ contact->GetFixtureB()->GetBody() };
	auto [player, other] = findPlayer_Other(bodyA, bodyB);

	if (player)
	{
		if (other->isA(GameObject::Type::trigger))
		{
			hitTrigger(other, player, contact, travelDirection(bodyB));
		}

		if (other->getComponent<InventoryComponent>())
		{
			PickUpItem(player, other);
		}

		if (other->isA(GameObject::Type::floor))
		{
			auto stop{ new StopUpdateDecorator(other,&devices, other->getComponent<SpriteComponent>(),player) };
			//other->getComponent<SpriteComponent>()->hide(player);
		}
		if (auto ghost{ other->getComponent<GhostComponent>() };
			ghost && ghost->canPass(travelDirection(bodyB))
			)
		{
			contact->SetEnabled(false);

			return;
		}

		if (other->isA(GameObject::Type::wall))
		{
			wallCollision(
				player->getComponent<BodyComponent>(),
				other->getComponent<BodyComponent>(),
				travelDirection(bodyB),
				&devices
			);
		}
	}
}
void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}


void ContactListener::PickUpItem(GameObject* player, GameObject* item)
{
	if(player -> getComponent<BackpackComponent>() -> pickUpItem(item))
	{
		devices.getSoundDevice() -> PlaySound(SoundEffect::Event::pickup,0,3);
	}
}
