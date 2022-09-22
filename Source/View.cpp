#include <iostream>
#include "View.h"

View::View(Vector2D centerPosition):position(centerPosition)
{

}

View::~View()
{
	
}


//**************************************
//NOT USED
bool View::Update(float gameTime)
//**************************************
{
	return true;
}

//**************************************
//print view data
void View::Print()
//**************************************
{
	std::cout << "*****VIEW******" << std::endl <<
		"View X: " << position.x << std::endl <<
		"View Y: " << position.y << std:: endl <<
		std::endl;
}