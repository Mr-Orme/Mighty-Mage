#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "Definitions.h"
class GraphicsDevice;
class Texture {

public:

    Texture(std::string path, GraphicsDevice* gDevice);
    Texture(SDL_Texture* texture);
    ~Texture();

    
   

    void run(SDL_Renderer* renderer, Vector2D position, float angle, SDL_Rect* clip = nullptr);

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