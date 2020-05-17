#include "HealthComponent.h"
#include "RendererComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"


HealthComponent::HealthComponent(std::shared_ptr<GameObject> owner):Component(owner){}
HealthComponent::~HealthComponent(){}

//**************************************
//gets health and resource manager from passed presets, the object starts out alive.
bool HealthComponent::Initialize(GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	isDead = false;
	devices = presets.devices;
	health = devices -> GetObjectLibrary() -> Search(presets.objectType).health;
	return true;
}

//**************************************
//This "KillObject" is for items that leave a sprite that is dead 
//and does not interact with the world
bool HealthComponent::KillObject(std::string deathSprite)
//**************************************
{
	//Stop the physics of the object
	devices -> GetPhysicsDevice() -> SetStopPhysics(_owner.get());

	//grab the renderer
	std::shared_ptr<RendererComponent> compRenderer = _owner -> GetComponent<RendererComponent>();
	//change the sprite
	compRenderer -> SetTexture(devices -> GetArtLibrary() -> Search(deathSprite));	
	return true;
}
//**************************************
//For objects that disappear from the game.
bool HealthComponent::KillObject()
//**************************************
{
	//this will cause it to be elimanted on the next game update
	isDead = true;
	return true;
}
void HealthComponent::Start()
{
	
}

//**************************************
//checks for death and deals with it
std::shared_ptr<GameObject> HealthComponent::Update()
//**************************************
{
	//if dead
	if(health <= 0)
	{
		//if this is a joined object
		if(_owner -> GetJoinedWith() != NULL)
		{
			//Turn off the joined object
			std::shared_ptr<GameObject> joined =  _owner -> GetJoinedWith();
			devices -> GetPhysicsDevice() -> SetStopPhysics(joined.get());
			//destroy the joints
			devices -> GetPhysicsDevice() -> DestroyJoint(_owner.get());
		}
		//kill it
		KillObject();
	}
	return NULL;
}
void HealthComponent::Finish(){}