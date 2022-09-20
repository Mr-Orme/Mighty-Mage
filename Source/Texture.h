#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "Definitions.h"

class Texture{

    public:
        //Initializes variables
        Texture();

        //Deallocates memory
        ~Texture();

        //Loads image at specified path
        bool load(SDL_Renderer* renderer, std::string path );
		bool load(SDL_Texture* texture);

        //Deallocates texture
        void free();

        //Renders texture at given point
		
		void run(SDL_Renderer* renderer, Vector2D position, float angle, SDL_Rect* clip = nullptr );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:

        //The actual hardware texture
        SDL_Texture* texture;

        //Image dimensions
        int width;
        int height;

};

#endif