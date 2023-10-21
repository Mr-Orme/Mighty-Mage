#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"


class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(ResourceManager* devices, std::string path);
	~SpriteComponent();

	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<Component> copyMe() const override;
	
	Texture* getTexture() const { return texture.get(); };
	void changeSprite(std::shared_ptr<Texture> texture);

protected:
	std::shared_ptr<Texture> texture{ nullptr };

};

#endif