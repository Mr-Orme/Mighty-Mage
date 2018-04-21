#include "BodyComponent.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

BodyComponent::BodyComponent(std::shared_ptr<GameObject> owner):Component(owner){}
BodyComponent::~BodyComponent(){}

//**************************************
//Based on the presets struct passed in, a fixture is created
bool BodyComponent::Initialize(GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	
	std::shared_ptr<RendererComponent> compRenderer = _owner -> GetComponent<RendererComponent>();
	GAME_PHYSICS physics;
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
	oldPosition = presets.position;
	return true;
}

void BodyComponent::Start(){}
//**************************************
//finds the current position, subtract's off the last frame's position
//to get the change in position. This is for the auto scrolling feature.
//the newpostion then becomes the old position.
std::shared_ptr<GameObject> BodyComponent::Update()
//**************************************
{
	GAME_VEC newPosition = devices -> GetPhysicsDevice() -> GetPosition(_owner.get());
	dPosition.x =  newPosition.x- oldPosition.x;
	dPosition.y =  newPosition.y- oldPosition.y;
	oldPosition = newPosition;
	return NULL;
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