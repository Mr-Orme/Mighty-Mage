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
bool View::Initialize(GAME_FLT x, GAME_FLT y)
//**************************************
{
	//this -> iDevice = iDevice;
	position.x = x;
	position.y = y;
	return true;
}

//**************************************
//NOT USED
bool View::Update(GAME_FLT gameTime)
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