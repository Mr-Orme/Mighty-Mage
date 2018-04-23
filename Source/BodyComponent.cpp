#include "BodyComponent.h"
#include "RendererComponent.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "PhysicsAssetLibrary.h"

BodyComponent::BodyComponent(std::shared_ptr<GameObject> owner):Component(owner){}
BodyComponent::~BodyComponent(){}

//**************************************
//Based on the presets struct passed in, a fixture is created
bool BodyComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	
	std::shared_ptr<RendererComponent> compRenderer = _owner -> GetComponent<RendererComponent>();
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
			_owner.get(),
			physics,
			presets
			);
	}	
	return true;
}

void BodyComponent::Start(){}

//**************************************
//
std::shared_ptr<GameObject> BodyComponent::Update()
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
	if(!devices -> GetPhysicsDevice() -> RemoveObject(_owner.get()))
	{
		printf( "Object could not be removed from Physics World");
		exit(1);					
	}
}
GAME_FLT BodyComponent::GetAngle()
{
	return devices->GetPhysicsDevice()->GetAngle(_owner.get());
}

GAME_VEC BodyComponent::getPosition()
{
	return (devices->GetPhysicsDevice()->GetPosition(_owner.get()));
}
GAME_VEC BodyComponent::getVelocity()
{
	return devices->GetPhysicsDevice()->GetVelocity(_owner.get());
}
void BodyComponent::setAngle(GAME_FLT angle)
{
	devices->GetPhysicsDevice()->SetAngle(_owner.get(), angle);
}

void BodyComponent::adjustAngle(GAME_FLT adjustAmount)
{
	setAngle(GetAngle() + adjustAmount);
}

void BodyComponent::linearStop()
{
	devices->GetPhysicsDevice()->SetLinearVelocity(_owner.get(), { 0,0 });
}
