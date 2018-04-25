#include "BodyComponent.h"
#include "RendererComponent.h"
#include "ResourceManager.h"
#include "PhysicsAssetLibrary.h"
#include "GameObject.h"
#include "PhysicsDevice.h"
#include "Texture.h"

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
GAME_FLT BodyComponent::getAngle()
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
GAME_INT BodyComponent::getWidth()
{
	return _owner->GetComponent<RendererComponent>()->GetTexture()->getWidth();
}
GAME_INT BodyComponent::getHeight()
{
	return _owner->GetComponent<RendererComponent>()->GetTexture()->getHeight();
}
void BodyComponent::setAngle(GAME_FLT angle)
{
	devices->GetPhysicsDevice()->SetAngle(_owner, angle);
}

void BodyComponent::adjustAngle(GAME_FLT adjustAmount)
{
	setAngle(getAngle() + adjustAmount);
}

void BodyComponent::linearStop()
{
	devices->GetPhysicsDevice()->SetLinearVelocity(_owner, { 0,0 });
}

//**************************************
//find's the 15x15 game square based on current position
GAME_VEC BodyComponent::getCurrentSquare()
//**************************************
{


	GAME_VEC cityCorner = devices->GetCityCorner();

	//subtract off the player's position on the screen to get the actual spot of the player.
	//divide by the number of pixels in each square.
	//Adjust the y, because the 15x15 square starts in the bottom left corner, while SDL starts in the top left.
	//TODO: needs to determine square by nose! adjust according to direction facing!
	GAME_VEC square =
	{
		int((cityCorner.x - getPosition().x)*-1 / squareDimension),
		15 + int((cityCorner.y - getPosition().y - _owner->GetComponent<RendererComponent>()->GetTexture()->getHeight()) / squareDimension)
	};
	return square;

}