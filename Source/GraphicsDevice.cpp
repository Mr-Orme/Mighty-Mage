#include<iostream>
#include "SDL2_gfxPrimitives.h"
#include "ResourceManager.h"
#include "ComponentsList.h"
#include "ObjectFactory.h"
#include "GraphicsDevice.h"
//#include "InputDevice.h"
#include "View.h"




GraphicsDevice::GraphicsDevice(Uint32 width, Uint32 height) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height)
{
	screen = NULL;
	renderer = NULL;
}

GraphicsDevice::~GraphicsDevice()
{
	/*if(!ShutDown())
	{
		printf( "SDL could not shut down! SDL_Error: %s\n", SDL_GetError() );
		exit(1);
	}*/
}

SDL_Window* GraphicsDevice::GetWindow()
{
	return(screen);
}

bool GraphicsDevice::Initialize(bool fullScreen)
{


	//Initialize all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return(false);
	}

	//Initialize SDL_image subsystems
	if(!IMG_Init(IMG_INIT_PNG))
	{
		printf( "SDL_image could not initialize! SDL_Error: %s\n", IMG_GetError() );
		return(false);
	}

	//Initialize SDL_ttf subsystems
	if(TTF_Init()==-1)
	{
		printf( "SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError() );
		return(false);
	}

	if(!fullScreen)
	{
		//Construct and check window construction
		screen = SDL_CreateWindow("Demonstration Window",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		screen = SDL_CreateWindow("Demonstration Window",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	}
	if(screen==NULL)
	{
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return(false);
	}

	//Construct the renderer
	renderer = SDL_CreateRenderer(screen,-1,SDL_RENDERER_ACCELERATED);
	if(renderer==NULL)
	{
		printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
		return(false);
	}

	//Set the background color (default)
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	//========================================
	//create view
	//========================================
	view = std::make_shared<View>();
	view -> Initialize (0, 0);

	return(true);

}

bool GraphicsDevice::ShutDown()
{
	//Free the window
	SDL_DestroyWindow(screen);
	screen = NULL;

	//Free renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	

	//Quit SDL Subsystems
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();


	return(true);
}




void GraphicsDevice::Begin()
{
	SDL_RenderClear(renderer);
}

void GraphicsDevice::Draw()
{
	//********************************draw level*************************************
	for(auto sprite : sprites)
	{
		sprite->Draw();
	}
	//*********************************************************************************



	//********************************draw overlays*************************************
	int cornerRadius = 3; //radius of curve of message box corners.
	//the text vector holds any messages.
	while(!overlays.empty())
	{

		overlay currOverlay = overlays.back();
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
		for( auto object : currOverlay.objects)
		{
			object.first ->Draw(renderer, object.second, 0, NULL);

		}

		
			
//!!!!!!!!!!!!!!!!!!!!!!!!!!this needs to be elsewhere!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	GAME_INT spheresFound = 0; //number of spheres found
//	//if we found all the spheres.
//	if(spheresFound >= 6)
//	{
//		ResourceManager* devices = levelExit -> GetComponent<BodyComponent>() -> GetDevices().get();
//		//stop the physics on the trapdoor so we can walk onto that square.
//		devices -> GetPhysicsDevice() -> SetStopPhysics(levelExit.get());
//		//get rid of the notice stating we need to find the spheres.
//		GAME_NOTICE notice = {15, 0, W, ""};
//		devices -> GetNoticesLibrary() -> RemoveAsset(notice);
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	}
	//back to black. . .
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
}

void GraphicsDevice::Present()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* GraphicsDevice::GetRenderer()
{
	return(renderer);
}

void GraphicsDevice::AddSpriteRenderer(RendererComponent* sprite)
{
	sprites.push_back(sprite);
}
void GraphicsDevice::RemoveSpriteRenderer(RendererComponent* dSprite)
{
	std::vector<RendererComponent*>::iterator spriteIter;
	for(spriteIter=sprites.begin(); spriteIter!=sprites.end(); spriteIter++)
	{
		if(*spriteIter == dSprite)
		{
			spriteIter = sprites.erase(spriteIter);
			
		}
	}
}
bool GraphicsDevice::SetFont(std::string path, GAME_INT size, GAME_RGBA color)
{
	font = TTF_OpenFont(path.c_str(), size);
	if( font == NULL )
	{
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	this -> color = color;

	return(true);

}

//**************************************
//adds text to be displayed to the text vector pased on a string and position
void GraphicsDevice::Text2Screen(std::string text, GAME_VEC position)
//**************************************
{
	int widthIncrease = 10; //left and right padding
	int heightIncrease = 2; //top and bottom padding

	
	//set color of text
	SDL_Color textColor = {color.R, color.G, color.B, color.A};

	//create texture
	SDL_Texture* textSheetTexture = SDL_CreateTextureFromSurface( 
															renderer,  
															TTF_RenderText_Solid(
																	font, 
																	text.c_str(), 
																	textColor));

	//create a texture for the game.
	Texture* textTexture = new Texture();
	textTexture -> load(textSheetTexture);

	int width=0, height=0;
	//grab textures' with and ehight.
	SDL_QueryTexture(textSheetTexture, NULL, NULL, &width, &height);
		//If we set a position of the box to -1, we center it.
		//bottomRight needs to be the width + 1;
	if(position.x == -1)
	{
		position.x = Center(SCREEN_WIDTH, width);
			
	}
	if(position.y == -1)
	{
		position.y = Center(SCREEN_HEIGHT, height);
	}

	GAME_VEC topLeft = {position.x - widthIncrease, position.y -heightIncrease};
	GAME_VEC bottomRight = {position.x + width + widthIncrease, position.y + height + heightIncrease};

	GAME_RGBA background = {255, 255, 255, 255};
	GAME_RGBA border = {0, 0, 0, 255};

	
	std::map<Texture*, GAME_VEC> objects;
	objects[textTexture] = position;

	DrawOverlay(topLeft, bottomRight, background, border, objects);

}
//**************************************
//adds text to be displayed to the text vector pased on a string and position
//this one let's us directly type the position's values
void GraphicsDevice::Text2Screen(std::string text, GAME_FLT x, GAME_FLT y)
//**************************************
{
	GAME_VEC position ={x,y};
	Text2Screen(text, position);
}

//**************************************
//a notice appears in the inventory bar at the bottom of the screen.
void GraphicsDevice::Notice2Screen(std::string text)
//**************************************
{
	GAME_VEC textVec = {-1,550};
	Text2Screen(text, textVec);

}

//**************************************
//reverses the order of sprites so that the player is on top.
void GraphicsDevice::ReverseOrder()
//**************************************
{
	std::reverse(sprites.begin(), sprites.end());
}
//**************************************
//draws a filled circle.
void GraphicsDevice::DrawFilledCircle(GAME_VEC position, GAME_INT radius, GAME_RGBA RGBA)
//**************************************
{
	filledCircleRGBA(renderer, position.x, position.y, radius, RGBA.R, RGBA.G, RGBA.B, RGBA.A);
}

bool GraphicsDevice::DrawBox(GAME_VEC topLeft, GAME_VEC bottomRight, GAME_RGBA RGBA)
{
	boxRGBA(renderer, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, RGBA.R, RGBA.G, RGBA.B,RGBA.A);
	return true;
}

void GraphicsDevice::DrawOverlay(GAME_VEC topLeft, GAME_VEC bottomRight, GAME_RGBA boxBackgroundColor, GAME_RGBA boxBorderColor, std::map<Texture*, GAME_VEC> objects)
{
	overlay newOverlay = {topLeft, bottomRight, boxBackgroundColor, boxBorderColor, objects};
	overlays.push_back(newOverlay);
}

GAME_FLT GraphicsDevice::Center(GAME_FLT centerOn, GAME_FLT width)
{
			
			
	GAME_FLT point = (centerOn - width)/2;
			
	return point;
}