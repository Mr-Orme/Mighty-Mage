/*
* Author:	Sean Orme
*			UALR
* 
* Date:		May, 2015	
*
* File:		GraphicsDevice.h
*
* Purpose:	Declaration of the SDL GraphicsDevice class
*/

#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include <vector>
#include <map>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "RendererComponent.h"
class View;
//class InputDevice;

class GraphicsDevice
{
		
public:

	//Constructors and Destructors
	GraphicsDevice(Uint32, Uint32);
	~GraphicsDevice();

	//Startup and Shutdown
	bool Initialize(bool);
	bool ShutDown();

	//Rendering functions
	void Begin();
	void Draw();
	void Present();

	void Text2Screen(std::string text, GAME_VEC position);
	void Text2Screen(std::string text, GAME_FLT x, GAME_FLT y);
	void Notice2Screen(std::string text);

	void DrawFilledCircle(GAME_VEC position, GAME_INT radius, GAME_RGBA RGBA);
	bool DrawBox(GAME_VEC topLeft, GAME_VEC bottomRight, GAME_RGBA RGBA);
	
	void DrawOverlay
		(
			//round corner box that contains overlay
			GAME_VEC topLeft, GAME_VEC bottomRight, GAME_RGBA boxBackgroundColor, GAME_RGBA boxBorderColor, 
			//any objects drawn in stated box
			//at given position
			std::map<Texture*, GAME_VEC> objects
			
		);

	void ReverseOrder();

	//!!!!!!!!!!!!!!!!!!!!!Move where exit is!!!!!!!!!!!!!!!!!
	std::shared_ptr<GameObject> GetExit(){ return levelExit; }

	//Accessors
	void AddSpriteRenderer(RendererComponent*);
	void RemoveSpriteRenderer(RendererComponent*);

	//Getters
	SDL_Renderer* GetRenderer();
	SDL_Window* GetWindow();
	GAME_INT GetScreenWidth(){return SCREEN_WIDTH;}
	GAME_INT GetScreenHeight(){return SCREEN_HEIGHT;}
	std::shared_ptr<View> GetView(){return view;}
	
	

	//Setters
	void SetView(std::shared_ptr<View> view){this -> view = view;}
	void SetExit(std::shared_ptr<GameObject> levelExit){this -> levelExit = levelExit;}
	bool SetFont(std::string path, GAME_INT size, GAME_RGBA color);



private:
	GAME_FLT Center(GAME_FLT centerOn, GAME_FLT width);
	//Parameters
	const Uint32 SCREEN_WIDTH;
	const Uint32 SCREEN_HEIGHT;

	//Window(s) to display graphics
	SDL_Window* screen;
	SDL_Renderer* renderer;

	//Renderers
	std::vector<RendererComponent*> sprites;

	std::shared_ptr<View> view;

	typedef struct overlay
	{
		GAME_VEC topLeft;
		GAME_VEC bottomRight;
		GAME_RGBA boxBackgroundColor;
		GAME_RGBA boxBorderColor;
		std::map<Texture*, GAME_VEC> objects;
	}overlay;

	std::vector<overlay> overlays;

	TTF_Font* font;
	GAME_RGBA color;

	std::shared_ptr<GameObject> levelExit;

};

#endif