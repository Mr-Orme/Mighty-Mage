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
	GraphicsDevice(Vector2D screenDimensions);
	~GraphicsDevice();

	//Startup and Shutdown
	bool initialize(bool);
	bool ShutDown();

	//Rendering functions
	void Begin();
	void run();
	void Present();

	void Text2Screen(std::string text, Vector2D position);
	void Text2Screen(std::string text, float x, float y);
	void Notice2Screen(std::string text);

	void DrawFilledCircle(Vector2D position, int radius, RGBA RGBA);
	bool DrawBox(Vector2D topLeft, Vector2D bottomRight, RGBA RGBA);
	struct Overlay
	{
		Vector2D topLeft;
		Vector2D bottomRight;
		RGBA boxBackgroundColor;
		RGBA boxBorderColor;
		std::map<Texture*, Vector2D> objects;
	};
	void addOverlay(Overlay overlay);

	void ReverseOrder();

	//TODO::!!!!!!!!!!!!!!!!!!!!!Move where exit is!!!!!!!!!!!!!!!!!
	GameObject* GetExit(){ return levelExit; }

	//Accessors
	void AddSpriteRenderer(RendererComponent*);
	void RemoveSpriteRenderer(RendererComponent*);

	//Getters
	SDL_Renderer* GetRenderer() { return renderer; }
	
	Vector2D getScreenDimensions() { return screenDimensions; }
	View* GetView(){return view.get(); }
	
	

	//Setters
		void SetExit(GameObject* levelExit){this -> levelExit = levelExit;}
	bool SetFont(std::string path, int size, RGBA color);



private:
	float Center(float centerOn, float width);
	//Parameters
	const Vector2D screenDimensions{};

	//Window(s) to display graphics
	SDL_Window* screen{ nullptr };
	SDL_Renderer* renderer{ nullptr };

	//Renderers
	//TODO::get rid of these
	std::vector<RendererComponent*> sprites;

	std::unique_ptr<View> view;

	

	std::vector<Overlay> overlays;
	std::vector<std::unique_ptr<Texture> > notices;
	TTF_Font* font{ nullptr };
	RGBA color{};

	GameObject* levelExit{ nullptr };

};

#endif