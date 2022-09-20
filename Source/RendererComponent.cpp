#include "RendererComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "ArtAssetLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include <memory>
RendererComponent::RendererComponent(GameObject* owner, ResourceManager* devices) :Component(owner, devices){}
RendererComponent::~RendererComponent(){}

//**************************************
//on the first pass, we set up the texture for the object
bool RendererComponent::initialize(ObjectFactoryPresets& presets)
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
	Vector2D updatedPosition;
	

	//adjust position.
	updatedPosition = GetUpdatedPosition(_owner);

	float angle = devices -> GetPhysicsDevice() -> getAngle(_owner);

	//Draw sprite.
	Draw(updatedPosition, angle);
}

void RendererComponent::Draw(Vector2D position, float angle)
{
	texture ->Draw(devices -> GetGraphicsDevice() -> GetRenderer(), position, angle, nullptr);
}

void RendererComponent::Start()
{

}

std::unique_ptr<GameObject> RendererComponent::update(){return nullptr;}

//**************************************
//adjusts the position based on the view.
Vector2D RendererComponent::GetUpdatedPosition(GameObject* owner)
//**************************************
{
	Vector2D updatedPosition;
	Vector2D position = devices -> GetPhysicsDevice() -> GetPosition(owner);
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