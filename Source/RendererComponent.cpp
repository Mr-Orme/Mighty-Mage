#include <memory>

#include "ObjectFactory.h"
#include "RendererComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "ArtAssetLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include "PhysicsDevice.h"
#include "GraphicsDevice.h"

RendererComponent::RendererComponent(std::shared_ptr<GameObject> owner):Component(owner){initialized = false;}
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
	
	//Draw sprite.
	Draw(GetViewAdjustedPosition(), _owner->GetComponent<BodyComponent>()->GetAngle());
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
GAME_VEC RendererComponent::GetViewAdjustedPosition()
//**************************************
{
	return _owner->GetComponent<BodyComponent>()->getPosition() + devices->GetGraphicsDevice()->GetView()->getPosition();
}

//**************************************
//
void RendererComponent::Finish()
//**************************************
{
	
}