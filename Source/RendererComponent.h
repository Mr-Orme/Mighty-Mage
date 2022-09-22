#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"
#include "Definitions.h"

class Texture;

class RendererComponent : public Component
{
public:
	RendererComponent(GameObject* owner, ResourceManager* devices);
	~RendererComponent();

	std::unique_ptr<GameObject> update() override;
	bool initialize(ObjectFactoryPresets& presets) override;
	
	void run();
	void run(Vector2D position, float angle);

	


	Texture* getTexture() const {return texture;}
	//TODO::move viewAdjustedPosition to BodyComponent
	Vector2D viewAdjustedPosition(GameObject*);

	void changeSprite(Texture* texture){this -> texture = texture;}

protected:
	Texture* texture{ nullptr };
	bool initialized{ false };
};

#endif