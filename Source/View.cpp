
#include <iostream>
#include "View.h"
#include "BodyComponent.h"
View::View(Vector2D centerPosition, const Vector2D& screenDimensions):
	position(centerPosition), screenDimensions(screenDimensions)
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

void View::borderDectection(Vector2D objectPosition)
{
	objectPosition = relativePosition(objectPosition);
	
	//left
	if (objectPosition.x < border)
	{
		setX(getViewingWindowPosition().x + (border-objectPosition.x));
	}
	//right
	else if (objectPosition.x >screenDimensions.x - border)
	{
		setX(getViewingWindowPosition().x - (objectPosition.x - (screenDimensions.x - border)));
	}
	//top
	if (objectPosition.y < border)
	{
		setY(getViewingWindowPosition().y + (border-objectPosition.y));
	}
	//bottom
	else if (objectPosition.y > screenDimensions.y - border)
	{
		setY(getViewingWindowPosition().y - (objectPosition.y - (screenDimensions.y - border)));
	}
}

Vector2D View::relativePosition(Vector2D objectPosition)
{
	return position + objectPosition;
}
