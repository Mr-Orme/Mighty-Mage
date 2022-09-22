#include "HealthComponent.h"
#include "RendererComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"


HealthComponent::HealthComponent(GameObject* owner, ResourceManager* devices):Component(owner, devices){}
HealthComponent::~HealthComponent(){}

//**************************************
//gets health and resource manager from passed presets, the object starts out alive.
bool HealthComponent::initialize(ObjectFactoryPresets& presets)
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
	devices -> GetPhysicsDevice() -> SetStopPhysics(_owner);

	//grab the renderer
	auto compRenderer{ _owner->getComponent<RendererComponent>() };
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
std::unique_ptr<GameObject> HealthComponent::update()
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
		KillObject();
	}
	return nullptr;
}
void HealthComponent::Finish(){}