#ifndef VIEW_H
#define VIEW_H

#include "Definitions.h"


class View{
public:
	View();
	~View();

	bool Initialize(GAME_FLT x, GAME_FLT y);
	bool Update(GAME_FLT gameTime);
	void Print();
	
	GAME_VEC getPosition(){return position;}

	void setPosition(GAME_VEC position){this -> position = position;}
	void setX(GAME_FLT x){this -> position.x = x;}
	void setY(GAME_FLT y){this -> position.y = y;}
private:
	GAME_VEC position;
};


#endif