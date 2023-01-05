#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"

#include <string>
#include <memory>
#include <vector>
#include <variant>
class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject* owner, ResourceManager* devices);
	SpriteComponent(ResourceManager* devices, std::string path);
	SpriteComponent() = delete;
	~SpriteComponent();

	std::unique_ptr<GameObject> update(std::vector<std::unique_ptr<GameObject>>& objects) override;
	bool initialize(ObjectFactoryPresets& presets) override;
	std::unique_ptr<Component> copyMe() const override;
	Texture* getTexture() const { return texture.get(); };
	void changeSprite(std::shared_ptr<Texture> texture);

protected:
	void run();//TODO::change name to draw!
	void run(Vector2D position, float angle);
	std::shared_ptr<Texture> texture{ nullptr };
	bool initialized{ false };
};

#endif