#ifndef VIEW_H
#define VIEW_H
#include <vector>
#include "Definitions.h"

class GraphicsDevice;
class GameObject;

class View{
public:
	View();
	~View();

	bool Initialize(GraphicsDevice* gDevice, GAME_FLT x, GAME_FLT y);
	bool Update();
	inline void Print();
	
	inline GAME_VEC getPosition(){return position;}

	void addScroller(GameObject* object);
	inline void setPosition(GAME_VEC position){this -> position = position;}
	inline void setX(GAME_FLT x){this -> position.x = x;}
	inline void setY(GAME_FLT y){this -> position.y = y;}
private:
	GAME_VEC position;
	std::vector<GameObject*> scrollers;
	static const GAME_INT border;
	GraphicsDevice* gDevice;
};


#endif