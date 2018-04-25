#include "BodyComponent.h"
#include "RendererComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "PhysicsAssetLibrary.h"
#include "GameObject.h"

BodyComponent::BodyComponent(GameObject* owner):Component(owner){}
BodyComponent::~BodyComponent(){}

//**************************************
//Based on the presets struct passed in, a fixture is created
bool BodyComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	
	RendererComponent* compRenderer = _owner -> GetComponent<RendererComponent>();
	PhysicsDevice::GAME_PHYSICS physics;
	if(compRenderer != NULL)
	{
		//store the resource manager.
		devices = presets.devices;
				
		//Get physics based on object type.
		physics = devices -> GetPhysicsLibrary() -> Search(presets.objectType);
				
		//Create fixture.
		devices -> GetPhysicsDevice() -> createFixture
			(
			_owner,
			physics,
			presets
			);
	}	
	return true;
}

void BodyComponent::Start(){}

//**************************************
//
GameObject* BodyComponent::Update()
//**************************************
{
	return nullptr;
}
//**************************************
//**************************************
//When this component is done, it destroys the body associated with the owner.
void BodyComponent::Finish()
//**************************************
{
	//remove the physics body
	if(!devices -> GetPhysicsDevice() -> RemoveObject(_owner))
	{
		printf( "Object could not be removed from Physics World");
		exit(1);					
	}
}
GAME_FLT BodyComponent::GetAngle()
{
	return devices->GetPhysicsDevice()->GetAngle(_owner);
}

GAME_VEC BodyComponent::getPosition()
{
	return (devices->GetPhysicsDevice()->GetPosition(_owner));
}
GAME_VEC BodyComponent::getVelocity()
{
	return devices->GetPhysicsDevice()->GetVelocity(_owner);
}
void BodyComponent::setAngle(GAME_FLT angle)
{
	devices->GetPhysicsDevice()->SetAngle(_owner, angle);
}

void BodyComponent::adjustAngle(GAME_FLT adjustAmount)
{
	setAngle(GetAngle() + adjustAmount);
}

void BodyComponent::linearStop()
{
	devices->GetPhysicsDevice()->SetLinearVelocity(_owner, { 0,0 });
}
