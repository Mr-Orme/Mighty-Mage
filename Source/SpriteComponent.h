#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"


class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject* owner, ResourceManager* devices);
	SpriteComponent(ResourceManager* devices, std::string path);
	~SpriteComponent();

	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<Component> copyMe() const override;
	
	void run();
	void run(Vector2D position, float angle);
	Texture* getTexture() const { return texture.get(); };
	void changeSprite(std::shared_ptr<Texture> texture);

protected:
	std::shared_ptr<Texture> texture{ nullptr };
	bool initialized{ false };
};

#endif