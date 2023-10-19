#include <string>
#include "ContactListener.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ComponentsList.h"
#include "Definitions.h"
#include "SoundDevice.h"
#include "GraphicsDevice.h" //TODO::get rid of this
#include "PhysicsDevice.h"
//#include <sstream>
#include <tuple>


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
		//contact->SetEnabled(false);
	}
}

std::tuple<GameObject*, GameObject*> ContactListener::getObjects(b2Contact* contact)
{
	auto bodyA{ contact->GetFixtureA()->GetBody() };
	auto bodyB{ contact->GetFixtureB()->GetBody() };

	auto objectA{ pDevice->findObject(bodyA) };
	auto objectB{ pDevice->findObject(bodyB) };

	if (objectB->isA(GameObject::Type::player))
	{
		auto temp{ objectB };
		objectB = objectA;
		objectA = temp;
	}
	return { objectA, objectB };
}

ContactListener::ContactListener(PhysicsDevice* pDevice)
	:pDevice(pDevice)
{
}
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	auto [player, collidedWith] = getObjects(contact);
	
		
	if (collidedWith->isA(GameObject::Type::trigger))
	{
		hitTrigger(collidedWith, player, contact, travelDirection(player));
	}
	//TODO:: wall collision becomes a trigger.
	else if (collidedWith->isA(GameObject::Type::wall))
	{
		
		if (auto ghost{ collidedWith->getComponent<GhostComponent>() };
			ghost != nullptr && ghost->canPass(travelDirection(player))
			)
		{
			contact->SetEnabled(false);
			return;
		}

		wallCollision(
			player->getComponent<BodyComponent>(),
			collidedWith->getComponent<BodyComponent>(),
			travelDirection(player)
		);
	}
	else if (collidedWith->getComponent<InventoryComponent>())
	{
		pickUpItem(player, collidedWith);
	}
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}


void ContactListener::pickUpItem(GameObject* player, GameObject* item)
{

	ResourceManager* devices = player -> getComponent<BodyComponent>() -> getDevices();

	if(player -> getComponent<BackpackComponent>() -> pickUpItem(item))
	{
		devices -> getSoundDevice() -> PlaySound(SoundEffect::Event::pickup,0,3);
	}

}

Direction ContactListener::travelDirection(GameObject* player) const
{
	auto bodyComp{ player->getComponent<BodyComponent>() };
	auto lv{ bodyComp->getLinearVelocity()};
	if (lv.x > 0) return Direction::E;
	if (lv.x < 0) return Direction::W;
	if (lv.y > 0) return Direction::S;
	if (lv.y < 0) return Direction::N;
	return bodyComp->getDirection();

}