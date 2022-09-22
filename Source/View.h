#ifndef VIEW_H
#define VIEW_H

#include "Definitions.h"


class View{
public:
	View(Vector2D centerPosition);
	~View();

	
	bool Update(float gameTime);
	void Print();
	
	Vector2D getPosition(){return position;}

	void setPosition(Vector2D position){this -> position = position;}
	void setX(int x){this -> position.x = x;}
	void setY(int y){this -> position.y = y;}
private:
	Vector2D position;
};


#endif