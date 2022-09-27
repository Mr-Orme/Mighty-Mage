#include "RendererComponent.h"
#include "BodyComponent.h"
#include "Texture.h"
#include "ArtAssetLibrary.h"
#include "View.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <memory>
RendererComponent::RendererComponent(GameObject* owner, ResourceManager* devices) :Component(owner, devices){}
RendererComponent::~RendererComponent()
{
	//devices->GetGraphicsDevice()->RemoveSpriteRenderer(this);
}

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
		//devices -> GetGraphicsDevice() -> AddSpriteRenderer(this);

		//grab the sprite from the library.
		texture = presets.devices -> GetArtLibrary() -> search(presets.objectType);
		initialized = true;
	}
	return true;
}
//**************************************
// Updates the position based on the view and draws the sprite
void RendererComponent::run()
//**************************************
{
	BodyComponent* body{ _owner->getComponent<BodyComponent>() };
	run(
		devices->GetGraphicsDevice()->getView()->relativePosition(body->getPosition()),
		body->getAngle());
}

void RendererComponent::run(Vector2D position, float angle)
{
	texture ->draw(position, angle);
}



std::unique_ptr<GameObject> RendererComponent::update(std::vector<std::unique_ptr<GameObject>>& objects)
{
	run();
	return nullptr;
}



