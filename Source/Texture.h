#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "Vector2D.h"

class GraphicsDevice;
class Texture {

public:

    Texture(std::string path, GraphicsDevice* gDevice);
    Texture(SDL_Texture* texture, GraphicsDevice* gDevice);
    ~Texture();

    
   

    void draw(Vector2D position, float angle, Vector2D dimensions = {0,0});

    Vector2D getDimensions() { return dimensions; }

    private:
        void load(SDL_Texture* texture);
        void load(std::string path);
        //The actual hardware texture
        SDL_Texture* texture{ nullptr };

        GraphicsDevice* gDevice{ nullptr };

        //Image dimensions
        Vector2D dimensions{};


};

#endif