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
	std::string objectAType = objectA -> getObjectType();
	std::string objectBType = objectB -> getObjectType();
	
	
		
		
		
	if(objectAType == "Player")
	{
		
		//if we found a pickupable item, grab it.
		if(objectB -> GetComponent<InventoryComponent>())
		{
			
			PickUpItem(objectA, objectB);
		}
		


	}
	
	//walls always put the player as object B
	if (objectBType == "Player")
	{
		//if it is a wall, than set the input componet's hit wall value to true.
		if(objectAType =="HWall" || objectAType =="VWall" )
		{
			objectB -> GetComponent<UserInputComponent>() -> SetWallHit(true);
		}
		//otherwise, false.
		else 
		{
			objectB -> GetComponent<UserInputComponent>() -> SetWallHit(false);
		}
		std::unique_ptr<GhostComponent> ghost =  objectA -> GetComponent<GhostComponent>();
		if(ghost != nullptr)
		{
			std::map<Direction, bool> ghostMap = ghost -> getGhostDirection();
			
			for(auto direction : ghostMap)
			{
				switch (direction.first)
				{
				case Direction::N:
					if(direction.second && bodyB -> GetLinearVelocity().y <=0) 
					{
						contact -> SetEnabled(false);
					}
					break;
				case Direction::S:
					if(direction.second && bodyB -> GetLinearVelocity().y >=0)
					{
						contact -> SetEnabled(false);
					}
					break;
				case Direction::E:
					if(direction.second && bodyB -> GetLinearVelocity().x >=0)
					{
						contact -> SetEnabled(false);
					}
					break;
				case Direction::W:
					if(direction.second && bodyB -> GetLinearVelocity().x <=0) 
					{
						contact -> SetEnabled(false);
					}
					break;
				default:
					break;
				}

					
				
			}

		}
	}

}

void ContactListener::PickUpItem(GameObject* player, GameObject* item)
{
	//grab the resource manager from the player's body component
	std::unique_ptr<ResourceManager> devices = player -> GetComponent<BodyComponent>() -> GetDevices();
	//if there is space to add it to the backpack, play the "found item" sound. . .
	if(player -> GetComponent<BackpackComponent>() -> AddItem(item))
	{
		devices -> GetSoundDevice() -> PlaySound("found",0,3);
	}

}