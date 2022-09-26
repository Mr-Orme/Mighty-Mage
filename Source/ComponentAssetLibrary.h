#ifndef COMPONENTASSETLIBRARY_H
#define COMPONENTASSETLIBRARY_H

#include <map>
#include <string>
#include <memory>
#include <vector>


class Component;
class GameObject;
class GraphicsDevice;
class ResourceManager;

class ComponentAssetLibrary{
public:
	enum class Components
	{
		body,
		health,
		renderer,
		userInput,
		backpack,
		inventory,
		ghost,
	};
	ComponentAssetLibrary(ResourceManager* devices);

	std::vector<std::unique_ptr<Component>> search(std::string name, GameObject* owner);
	bool addAsset(std::string name, std::vector<Components> componentList);
private:
			//name					List of components
	std::map<std::string, std::vector<Components>> library;
	ResourceManager* devices{ nullptr };

};


#endif