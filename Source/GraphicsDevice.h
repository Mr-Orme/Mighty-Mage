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
	GraphicsDevice(int, int);
	~GraphicsDevice();

	//Startup and Shutdown
	bool initialize(bool);
	bool ShutDown();

	//Rendering functions
	void Begin();
	void Draw();
	void Present();

	void Text2Screen(std::string text, Vector2D position);
	void Text2Screen(std::string text, float x, float y);
	void Notice2Screen(std::string text);

	void DrawFilledCircle(Vector2D position, int radius, RGBA RGBA);
	bool DrawBox(Vector2D topLeft, Vector2D bottomRight, RGBA RGBA);
	
	void DrawOverlay
		(
			//round corner box that contains overlay
			Vector2D topLeft, Vector2D bottomRight, RGBA boxBackgroundColor, RGBA boxBorderColor, 
			//any objects drawn in stated box
			//at given position
			std::map<Texture*, Vector2D> objects
			
		);

	void ReverseOrder();

	//!!!!!!!!!!!!!!!!!!!!!Move where exit is!!!!!!!!!!!!!!!!!
	std::unique_ptr<GameObject> GetExit(){ return levelExit; }

	//Accessors
	void AddSpriteRenderer(RendererComponent*);
	void RemoveSpriteRenderer(RendererComponent*);

	//Getters
	SDL_Renderer* GetRenderer();
	SDL_Window* GetWindow();
	int GetScreenWidth(){return SCREEN_WIDTH;}
	int GetScreenHeight(){return SCREEN_HEIGHT;}
	std::unique_ptr<View> GetView(){return view;}
	
	

	//Setters
	void SetView(std::unique_ptr<View> view){this -> view = view;}
	void SetExit(std::unique_ptr<GameObject> levelExit){this -> levelExit = levelExit;}
	bool SetFont(std::string path, int size, RGBA color);



private:
	float Center(float centerOn, float width);
	//Parameters
	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;

	//Window(s) to display graphics
	SDL_Window* screen;
	SDL_Renderer* renderer;

	//Renderers
	std::vector<RendererComponent*> sprites;

	std::unique_ptr<View> view;

	typedef struct overlay
	{
		Vector2D topLeft;
		Vector2D bottomRight;
		RGBA boxBackgroundColor;
		RGBA boxBorderColor;
		std::map<Texture*, Vector2D> objects;
	}overlay;

	std::vector<overlay> overlays;

	TTF_Font* font;
	RGBA color;

	std::unique_ptr<GameObject> levelExit;

};

#endif