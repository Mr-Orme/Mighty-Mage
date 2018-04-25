#include <memory>
#include "RendererComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "ArtAssetLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "GraphicsDevice.h"

RendererComponent::RendererComponent(GameObject* owner):Component(owner)
{initialized = false;}
RendererComponent::~RendererComponent(){}

//**************************************
//on the first pass, we set up the texture for the object
bool RendererComponent::Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	//this will get hit twice, so we only want it done once.
	if(initialized == false)
	{
		devices = presets.devices;
		//Set this as the renderer for this object
		//devices -> GetGraphicsDevice() -> AddSpriteRenderer(this);

		//grab the sprite from the library.
		texture = presets.devices -> GetArtLibrary() -> Search(presets.objectType);
		initialized = true;
	}
	return true;
}
//**************************************
// Updates the position based on the view and draws the sprite
void RendererComponent::Draw()
//**************************************
{
	GAME_VEC updatedPosition;
	

	//adjust position.
	updatedPosition = GetViewAdjustedPosition();

	GAME_FLT angle = devices -> GetPhysicsDevice() -> GetAngle(_owner);

	//Draw sprite.
	Draw(updatedPosition, angle);
}

void RendererComponent::Draw(GAME_VEC position, GAME_FLT angle)
{
	texture ->Draw(devices -> GetGraphicsDevice() -> GetRenderer(), position, angle, NULL);
}

void RendererComponent::Start()
{

}

GameObject* RendererComponent::Update(){return nullptr;}

//**************************************
//adjusts the position based on the view.
GAME_VEC RendererComponent::GetViewAdjustedPosition()
//**************************************
{
	
	GAME_VEC position = devices -> GetPhysicsDevice() -> GetPosition(_owner);
	
	//adjust position.
	GAME_VEC updatedPosition = position + devices->GetGraphicsDevice()->GetView()->getPosition();

	
	return updatedPosition;
}

//**************************************
//When dead, must remove the sprite from the list
void RendererComponent::Finish()
//**************************************
{
	//devices -> GetGraphicsDevice() -> RemoveSpriteRenderer(this);
}