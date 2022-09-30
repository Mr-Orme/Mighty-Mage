#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL_Mixer.h"

#include "Vector2D.h"
#include "Definitions.h"
class View;
class SpriteComponent;
class Texture;


class GraphicsDevice
{		
public:

	//Constructors and Destructors
	GraphicsDevice(
		Vector2D screenDimensions, 
		std::string fontPath,
		int fontSize,
		RGBA fontColor,
		bool fullScreen = false);
	~GraphicsDevice();

	//Rendering functions
	void begin();
	void drawOverlays();
	void present();

	void text2Screen(std::string text, Vector2D position);
	void notice2Screen(std::string text);

	void drawFilledCircle(Vector2D position, int radius, RGBA color);
	bool drawBox(Vector2D topLeft, Vector2D bottomRight, RGBA color);
	void drawLine(Vector2D start, Vector2D end, RGBA color);
	void drawSprite(SDL_Texture* texture, Vector2D position, Vector2D dimensions, float angle, Vector2D clipDimensions);
	
	void setDrawColor(RGBA color);
	struct Overlay
	{
		Vector2D topLeft;
		Vector2D bottomRight;
		RGBA boxBackgroundColor;
		RGBA boxBorderColor;
		std::map<Texture*, Vector2D> objects;
	};
	void addOverlay(Overlay overlay);
	//TODO::z-order
	//void reverseOrder();

	SDL_Texture* createTexture(SDL_Surface* surface);
	Vector2D getScreenDimensions() { return screenDimensions; }
	View* getView();
	
	//Setters
	bool setFont(std::string path, int size, RGBA color);



private:
	int center(int centerOn, int width);
	//Parameters
	const Vector2D screenDimensions{};

	//Window(s) to display graphics
	SDL_Window* screen{ nullptr };
	SDL_Renderer* renderer{ nullptr };

	std::unique_ptr<View> view;
	
	std::vector<Overlay> overlays;
	std::vector<std::unique_ptr<Texture> > notices;
	TTF_Font* font{ nullptr };
	RGBA color{};

	

};

#endif