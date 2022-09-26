#include<iostream>
#include <map>
#include <vector>
#include "SDL2_gfxPrimitives.h"
#include "ResourceManager.h"
#include "ComponentsList.h"
#include "ObjectFactory.h"
#include "GraphicsDevice.h"
//#include "InputDevice.h"
#include "View.h"

#include "Texture.h"



GraphicsDevice::GraphicsDevice(
	Vector2D screenDimensions, 
	std::string fontPath,
	int fontSize,
	RGBA fontColor, 
	bool fullScreen) :
	screenDimensions(screenDimensions)
{

	//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	//Initialize SDL_image subsystems
	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf("SDL_image could not initialize! SDL_Error: %s\n", IMG_GetError());
		return;
	}

	//Initialize SDL_ttf subsystems
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError());
		return;
	}

	if (fullScreen)
	{
		//Construct and check window construction
		screen = SDL_CreateWindow("Demonstration Window",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			screenDimensions.x, screenDimensions.y, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		screen = SDL_CreateWindow("Demonstration Window",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			screenDimensions.x, screenDimensions.y, SDL_WINDOW_SHOWN);

	}
	if (screen == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	//Construct the renderer
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	//Set the background color (default)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//========================================
	//create view
	//========================================
	Vector2D startPosition{ 0,0 };
	view = std::make_unique<View>(startPosition, this->screenDimensions);

	setFont(fontPath, fontSize, fontColor);

}

GraphicsDevice::~GraphicsDevice()
{
	//Free the window
	SDL_DestroyWindow(screen);
	screen = nullptr;

	//Free renderer
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	

	//Quit SDL Subsystems
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}






void GraphicsDevice::begin()
{
	SDL_RenderClear(renderer);
}

void GraphicsDevice::drawOverlays()
{
	
	int cornerRadius = 3; //radius of curve of message box corners.
	//the text vector holds any messages.
	while(!overlays.empty())
	{

		Overlay currOverlay{ std::move(overlays.back()) };
		overlays.pop_back();

		
		//draw the box
		roundedBoxRGBA
		(
			renderer, 
			currOverlay.topLeft.x, 
			currOverlay.topLeft.y, 
			currOverlay.bottomRight.x, 
			currOverlay.bottomRight.y, 
			cornerRadius, 
			currOverlay.boxBackgroundColor.R, 
			currOverlay.boxBackgroundColor.G, 
			currOverlay.boxBackgroundColor.B, 
			currOverlay.boxBackgroundColor.A
		);
		//draw the border
		roundedRectangleRGBA
		(
			renderer, 
			currOverlay.topLeft.x, 
			currOverlay.topLeft.y, 
			currOverlay.bottomRight.x, 
			currOverlay.bottomRight.y, 
			cornerRadius, 
			currOverlay.boxBorderColor.R, 
			currOverlay.boxBorderColor.G, 
			currOverlay.boxBorderColor.B, 
			currOverlay.boxBorderColor.A
		);

		//draw the objects
		for( auto& object : currOverlay.objects)
		{
			object.first ->run(renderer, object.second, 0, nullptr);

		}

		
			
//TODO::!!!!!!!!!!!!!!!!!!!!!!!!!!this needs to be elsewhere!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	int spheresFound = 0; //number of spheres found
//	//if we found all the spheres.
//	if(spheresFound >= 6)
//	{
//		ResourceManager* devices = levelExit -> getComponent<BodyComponent>() -> getDevices().get();
//		//stop the physics on the trapdoor so we can walk onto that square.
//		devices -> GetPhysicsDevice() -> SetStopPhysics(levelExit.get());
//		//get rid of the notice stating we need to find the spheres.
//		Notice notice = {15, 0, W, ""};
//		devices -> GetNoticesLibrary() -> removeAsset(notice);
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	}
	//back to black. . .
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
}

void GraphicsDevice::present()
{
	SDL_RenderPresent(renderer);
	notices.clear();
}

//void GraphicsDevice::AddSpriteRenderer(RendererComponent* sprite)
//{
//	sprites.push_back(sprite);
//}
//void GraphicsDevice::RemoveSpriteRenderer(RendererComponent* dSprite)
//{
//	std::vector<RendererComponent*>::iterator spriteIter;
//	for(spriteIter=sprites.begin(); spriteIter!=sprites.end(); spriteIter++)
//	{
//		if(*spriteIter == dSprite)
//		{
//			spriteIter = sprites.erase(spriteIter);
//			
//		}
//	}
//}
bool GraphicsDevice::setFont(std::string path, int size, RGBA color)
{
	font = TTF_OpenFont(path.c_str(), size);
	if( font == nullptr )
	{
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	this -> color = color;

	return(true);

}

//**************************************
//adds text to be displayed to the text vector pased on a string and position
void GraphicsDevice::text2Screen(std::string text, Vector2D position)
//**************************************
{
	int widthIncrease = 10; //left and right padding
	int heightIncrease = 2; //top and bottom padding

	
	//set color of text
	SDL_Color textColor {(Uint8)color.R, (Uint8)color.G, (Uint8)color.B, (Uint8)color.A};

	//create texture
	SDL_Texture* textSheetTexture = SDL_CreateTextureFromSurface( 
															renderer,  
															TTF_RenderText_Solid(
																	font, 
																	text.c_str(), 
																	textColor));
	Vector2D dimensions;
	//grab textures' with and ehight.
	SDL_QueryTexture(textSheetTexture, nullptr, nullptr, &dimensions.x, &dimensions.y);
		//If we set a position of the box to -1, we center it.
		//bottomRight needs to be the width + 1;
	if(position.x == -1)
	{
		position.x = center(screenDimensions.x, dimensions.x);
			
	}
	if(position.y == -1)
	{
		position.y = center(screenDimensions.y, dimensions.y);
	}

	Vector2D topLeft = {position.x - widthIncrease, position.y -heightIncrease};
	Vector2D bottomRight = {position.x + dimensions.x + widthIncrease, position.y + dimensions.y + heightIncrease};

	RGBA background = {255, 255, 255, 255};
	RGBA border = {0, 0, 0, 255};

	notices.push_back(std::make_unique<Texture>(textSheetTexture));
	addOverlay(
		{ 
			topLeft, 
			bottomRight, 
			background, 
			border, 
			{ {notices.back().get(), position} } 
		});

}
//**************************************
//adds text to be displayed to the text vector pased on a string and position
//this one let's us directly type the position's values
void GraphicsDevice::text2Screen(std::string text, float x, float y)
//**************************************
{
	Vector2D position ={x,y};
	text2Screen(text, position);
}

//**************************************
//a notice appears in the inventory bar at the bottom of the screen.
void GraphicsDevice::notice2Screen(std::string text)
//**************************************
{
	Vector2D textVec = {-1,550};
	text2Screen(text, textVec);

}

//**************************************
//draws a filled circle.
void GraphicsDevice::drawFilledCircle(Vector2D position, int radius, RGBA color)
//**************************************
{
	filledCircleRGBA(renderer, position.x, position.y, radius, color.R, color.G, color.B, color.A);
}

bool GraphicsDevice::drawBox(Vector2D topLeft, Vector2D bottomRight, RGBA color)
{
	boxRGBA(renderer, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, color.R, color.G, color.B, color.A);
	return true;
}

void GraphicsDevice::drawLine(Vector2D start, Vector2D end, RGBA color)
{
	lineRGBA(renderer, start.x,start.y,end.x,end.y,color.R, color.G, color.B, color.A);
}

void GraphicsDevice::addOverlay(Overlay overlay )
{
	
	overlays.push_back(std::move(overlay));
}

float GraphicsDevice::center(float centerOn, float width)
{
			
			
	float point = (centerOn - width)/2;
			
	return point;
}