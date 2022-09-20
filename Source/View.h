#ifndef VIEW_H
#define VIEW_H

#include "Definitions.h"


class View{
public:
	View();
	~View();

	bool initialize(float x, float y);
	bool Update(float gameTime);
	void Print();
	
	Vector2D getPosition(){return position;}

	void setPosition(Vector2D position){this -> position = position;}
	void setX(float x){this -> position.x = x;}
	void setY(float y){this -> position.y = y;}
private:
	Vector2D position;
};


#endif