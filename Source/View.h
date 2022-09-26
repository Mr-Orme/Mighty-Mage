#ifndef VIEW_H
#define VIEW_H

#include "Vector2D.h"
class BodyComponent;

class View{
public:
	View(Vector2D centerPosition, const Vector2D& screenDimensions);
	~View();

	
	bool Update(float gameTime);
	void Print();
	void borderDectection(Vector2D position);
	//TODO:: rename. relative vs actual?
	Vector2D relativePosition(Vector2D position);
	Vector2D getViewingWindowPosition(){return position;}

	void setPosition(Vector2D position){this -> position = position;}
	void setX(int x){this -> position.x = x;}
	void setY(int y){this -> position.y = y;}
private:
	Vector2D position{};
	const Vector2D& screenDimensions;
	const int border{ 200 };
};


#endif