#include <iostream>
#include "Texture.h"
#include "GraphicsDevice.h"

Texture::Texture(std::string path, GraphicsDevice* gDevice):gDevice(gDevice)
{
	load(path);
}

Texture::Texture(SDL_Texture* texture, GraphicsDevice* gDevice):gDevice(gDevice)
{
	load(texture);
}


Texture::~Texture()
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}


void Texture::load(std::string path )
{

	//The image that's loaded
	SDL_Surface* surface{ IMG_Load(path.c_str()) };
    
    //If the image loaded
	if( surface == nullptr)
	{
   		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );

	}
	else
	{
		 //Color key image
        SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 255, 0, 255 ) );

		//Create an optimized image
		texture = gDevice->createTexture(surface);
     
		if(texture == nullptr)
		{
			printf( "Unable to create texture from image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );

		}
		else
		{

            //Set the height and width from the texture
			SDL_QueryTexture(texture,nullptr,nullptr,&dimensions.x,&dimensions.y);

			//Free the loaded image; no longer needed
			SDL_FreeSurface(surface);

		}

	}



}

void Texture::load(SDL_Texture* texture)
{
	this -> texture = texture;
	//Set the height and width from the texture
	SDL_QueryTexture(texture,nullptr,nullptr, &dimensions.x, &dimensions.y);
	
}
	   

void Texture::draw(Vector2D position, float angle, Vector2D clipDimensions)
{
	gDevice->drawSprite(texture, position, dimensions, angle, clipDimensions);
	

}
