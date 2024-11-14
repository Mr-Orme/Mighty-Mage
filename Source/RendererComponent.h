#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"
#include "Definitions.h"

class Texture;

class RendererComponent : public Component
{
public:
	RendererComponent(std::shared_ptr<GameObject>);
	~RendererComponent();

	bool Initialize(GAME_OBJECTFACTORY_PRESETS& presets);
	
	void Draw();
	void Draw(GAME_VEC position, GAME_FLT angle);
	void Start();
	std::shared_ptr<GameObject> Update();
	void Finish();

	Texture* GetTexture(){return texture;}
	GAME_VEC GetUpdatedPosition(std::shared_ptr<GameObject>);

	void SetTexture(Texture* texture){this -> texture = texture;}

protected:
	std::shared_ptr<ResourceManager> devices;
	Texture* texture;
	bool initialized;
};

#endif