#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include <vector>
#include <map>
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL2_gfxPrimitives.h"

#include "Vector2D.h"
#include "BodyComponent.h"
class View;
class RendererComponent;
class Texture;
struct RGBA
{
	int R;
	int G;
	int B;
	int A;
};
struct Notice
{
	Vector2D square;
	BodyComponent::Direction direction;
	std::string text;
};
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
	void text2Screen(std::string text, float x, float y);
	void notice2Screen(std::string text);

	void drawFilledCircle(Vector2D position, int radius, RGBA color);
	bool drawBox(Vector2D topLeft, Vector2D bottomRight, RGBA color);
	void drawLine(Vector2D start, Vector2D end, RGBA color);
	
	
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

	//TODO::get rid of getRenderer.... Maybe pass in texture?
	SDL_Renderer* getRenderer() { return renderer; }
	Vector2D getScreenDimensions() { return screenDimensions; }
	View* getView(){return view.get(); }
	
	//Setters
	bool setFont(std::string path, int size, RGBA color);



private:
	float center(float centerOn, float width);
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