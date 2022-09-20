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

	bool initialize(ObjectFactoryPresets& presets);
	
	void Draw();
	void Draw(Vector2D position, float angle);
	void Start();
	std::unique_ptr<GameObject> update();
	void Finish();

	Texture* GetTexture(){return texture;}
	Vector2D GetUpdatedPosition(GameObject*);

	void SetTexture(Texture* texture){this -> texture = texture;}

protected:
	Texture* texture{ nullptr };
	bool initialized{ false };
};

#endif