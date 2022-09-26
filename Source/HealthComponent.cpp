#include "HealthComponent.h"
#include "RendererComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "GameObject.h"


HealthComponent::HealthComponent(GameObject* owner, ResourceManager* devices)
	:Component(owner, devices){}
HealthComponent::~HealthComponent(){}

//**************************************
//gets health and resource manager from passed presets, the object starts out alive.
bool HealthComponent::initialize(ObjectFactory::Presets& presets)
//**************************************
{
	devices = presets.devices;
	health = devices -> GetObjectLibrary() -> search(presets.objectType).health;
	return true;
}

//**************************************
//This "killMe" is for items that leave a sprite that is dead 
//and does not interact with the world
bool HealthComponent::killMe(std::string deathSprite)
//**************************************
{
	//Stop the physics of the object
	devices -> GetPhysicsDevice() -> SetStopPhysics(_owner);

	//grab the renderer
	auto compRenderer{ _owner->getComponent<RendererComponent>() };
	//change the sprite
	compRenderer -> changeSprite(devices -> GetArtLibrary() -> search(deathSprite));	
	return true;
}
//**************************************
//For objects that disappear from the game.
bool HealthComponent::killMe()
//**************************************
{
	//this will cause it to be elimanted on the next game update
	dead = true;
	return true;
}

//**************************************
//checks for death and deals with it
std::unique_ptr<GameObject> HealthComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
//**************************************
{
	//if dead
	if(health <= 0)
	{
		//if this is a joined object
		if(_owner -> getJoinedWith() != nullptr)
		{
			//Turn off the joined object
			GameObject* joined =  _owner -> getJoinedWith();
			devices -> GetPhysicsDevice() -> SetStopPhysics(joined);
			//destroy the joints
			devices -> GetPhysicsDevice() -> DestroyJoint(_owner);
		}
		//kill it
		killMe();
	}
	return nullptr;
}
