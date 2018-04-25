#include <iostream>
#include "View.h"
#include "GameObject.h"
#include "ComponentsList.h"
#include "GraphicsDevice.h"

const GAME_INT View::border = 200;

View::View()
{

}

View::~View()
{
	
}

//**************************************
//Set initial values
bool View::Initialize(GraphicsDevice* gDevice, GAME_FLT x, GAME_FLT y)
//**************************************
{
	position = { x,y };
	this->gDevice = gDevice;

	return true;
}

//**************************************
//NOT USED
bool View::Update()
//**************************************
{
	for (const auto& object : scrollers)
	{

		const GAME_VEC currentPosition = object->GetComponent<RendererComponent>()->GetViewAdjustedPosition();

		//left
		if (currentPosition.x < border)
		{
			this->setX(this->position.x + (border - currentPosition.x));
			
		}
		//right
		else if (currentPosition.x > gDevice->GetScreenWidth() - border)
		{
			this->setX(this->position.x - (currentPosition.x- (gDevice->GetScreenWidth() - border)));
		}
		//top
		if (currentPosition.y < border)
		{
			this->setY(this->position.y + (border - currentPosition.y));
		}
		//bottom
		else if (currentPosition.y > gDevice->GetScreenHeight() - border)
		{
			this->setY(this->position.y - (currentPosition.y - (gDevice->GetScreenHeight() - border)));
		}
		
	}
	//*********************************************************************************
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

void View::addScroller(GameObject * object)
{
	scrollers.push_back(object);
}
