#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"
#include "Definitions.h"

class Texture;
class ObjectFactory;
class RendererComponent : public Component
{
public:
	RendererComponent(GameObject*);
	~RendererComponent();

	bool Initialize(ObjectFactory::GAME_OBJECTFACTORY_PRESETS& presets);
	
	void Draw();
	void Draw(GAME_VEC position, GAME_FLT angle);
	void Start();
	GameObject* Update();
	void Finish();

	Texture* GetTexture(){return texture;}
	GAME_VEC GetViewAdjustedPosition(GameObject*);

	void SetTexture(Texture* texture){this -> texture = texture;}

protected:
	ResourceManager* devices;
	Texture* texture;
	bool initialized;
};

#endif