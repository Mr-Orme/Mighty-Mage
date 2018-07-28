#include "HealthComponent.h"
#include "RendererComponent.h"
#include "BodyComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "AssetLibrary.h"
#include "GameObject.h"

HealthComponent::HealthComponent(GameObject* owner):Component(owner){}
HealthComponent::~HealthComponent(){}

//**************************************
//gets health and resource manager from passed presets, the object starts out alive.
bool HealthComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	isDead = false;
	devices = presets.devices;
	health = devices -> getAssetLibrary() -> getObjectStats(presets.objectType).health;
	return true;
}

//**************************************
//This "KillObject" is for items that leave a sprite that is dead 
//and does not interact with the world
bool HealthComponent::KillObject(std::string deathSprite)
//**************************************
{
	//Stop the physics of the object
	devices -> getPhysicsDevice() -> SetStopPhysics(_owner);

	//grab the renderer
	RendererComponent* compRenderer = _owner -> GetComponent<RendererComponent>();
	//change the sprite
	compRenderer -> SetTexture(devices -> getAssetLibrary() -> getArtAsset(deathSprite));	
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
GameObject* HealthComponent::Update()
//**************************************
{
	//if dead
	if(health <= 0)
	{
		//if this is a joined object
		if(_owner -> GetJoinedWith() != NULL)
		{
			//Turn off the joined object
			GameObject* joined =  _owner -> GetJoinedWith();
			devices -> getPhysicsDevice() -> SetStopPhysics(joined);
			//destroy the joints
			devices -> getPhysicsDevice() -> DestroyJoint(_owner);
		}
		//kill it
		KillObject();
	}
	return NULL;
}
void HealthComponent::Finish(){}