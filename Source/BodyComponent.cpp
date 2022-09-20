#include "BodyComponent.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

BodyComponent::BodyComponent(GameObject* owner, ResourceManager* devices):Component(owner, devices){}
BodyComponent::~BodyComponent()
{
	//remove the physics body
	if (!devices->GetPhysicsDevice()->RemoveObject(_owner))
	{
		printf("Object could not be removed from Physics World");
		exit(1);
	}
}

//**************************************
//Based on the presets struct passed in, a fixture is created
bool BodyComponent::initialize(ObjectFactoryPresets& presets)
//**************************************
{
	PhysicsStats physics;
	if(_owner->getComponent<RendererComponent>() != nullptr)
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
	oldPosition = presets.position;
	return true;
}

//**************************************
//finds the current position, subtract's off the last frame's position
//to get the change in position. This is for the auto scrolling feature.
//the newpostion then becomes the old position.
std::unique_ptr<GameObject> BodyComponent::update()
//**************************************
{
	Vector2D newPosition = devices -> GetPhysicsDevice() -> GetPosition(_owner);
	dPosition.x =  newPosition.x- oldPosition.x;
	dPosition.y =  newPosition.y- oldPosition.y;
	oldPosition = newPosition;
	return nullptr;
}
