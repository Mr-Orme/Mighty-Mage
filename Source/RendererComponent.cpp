#include "RendererComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "ArtAssetLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include <memory>
RendererComponent::RendererComponent(std::shared_ptr<GameObject> owner):Component(owner){initialized = false;}
RendererComponent::~RendererComponent(){}

//**************************************
//on the first pass, we set up the texture for the object
bool RendererComponent::Initialize(GAME_OBJECTFACTORY_PRESETS& presets)
//**************************************
{
	//this will get hit twice, so we only want it done once.
	if(initialized == false)
	{
		devices = presets.devices;
		//Set this as the renderer for this object
		devices -> GetGraphicsDevice() -> AddSpriteRenderer(this);

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
	updatedPosition = GetUpdatedPosition(_owner);

	GAME_FLT angle = devices -> GetPhysicsDevice() -> GetAngle(_owner.get());

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

std::shared_ptr<GameObject> RendererComponent::Update(){return NULL;}

//**************************************
//adjusts the position based on the view.
GAME_VEC RendererComponent::GetUpdatedPosition(std::shared_ptr<GameObject> owner)
//**************************************
{
	GAME_VEC updatedPosition;
	GAME_VEC position = devices -> GetPhysicsDevice() -> GetPosition(owner.get());
	//adjust position.
	updatedPosition.x = 
		  position.x
		+ devices -> GetGraphicsDevice() -> GetView() -> getPosition().x;
	updatedPosition.y = 
		  position.y
		+ devices -> GetGraphicsDevice() -> GetView() -> getPosition().y;
	
	return updatedPosition;
}

//**************************************
//When dead, must remove the sprite from the list
void RendererComponent::Finish()
//**************************************
{
	devices -> GetGraphicsDevice() -> RemoveSpriteRenderer(this);
}