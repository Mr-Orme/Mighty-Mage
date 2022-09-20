#include <iostream>
#include "View.h"

View::View()
{

}

View::~View()
{
	
}

//**************************************
//Set initial values
bool View::initialize(float x, float y)
//**************************************
{
	//this -> iDevice = iDevice;
	position.x = x;
	position.y = y;
	return true;
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