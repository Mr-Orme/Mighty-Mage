#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"


class Texture;

class RendererComponent : public Component
{
public:
	RendererComponent(GameObject* owner, ResourceManager* devices);
	~RendererComponent();

	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	bool initialize(ObjectFactory::Presets& presets) override;
	
	void run();
	void run(Vector2D position, float angle);
	Texture* getTexture() const {return texture;}
	void changeSprite(Texture* texture){this -> texture = texture;}

protected:
	Texture* texture{ nullptr };
	bool initialized{ false };
};

#endif