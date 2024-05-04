#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "dilatation_and_erosion.h"

/*dilatation : fix the of the pixels with the max of his neighbourds
param : list of pixels, index, width, height, format of the image
*/ 

Uint32 dilatation(Uint32* pixels, int x, 
                    int width, int height, SDL_PixelFormat* format)
{
	int placeX = x / width;
	int placeY = x % width;
	
	Uint8 r, g, b;
	
	Uint8 max = 0;

	for (int i = - NEIGHBOURD; i <= NEIGHBOURD; i++)
	{
		for (int j = - NEIGHBOURD; j <= NEIGHBOURD; j++)
		{
			if ((i == 0 || j == 0) &&
			    placeX + i >= 0 && placeX + i < height &&
				placeY + j >= 0 && placeY + j < width)
			{
				SDL_GetRGB(
                    pixels[((placeX+i)*width)+placeY+j], format, &r, &g, &b);
                    
				max = (max < r) ? r : max;
			}
		}
	}
	
	//SDL_GetRGB(pixels[x], format, &r, &g, &b);
	//max = max-r;

	return SDL_MapRGB(format, max, max, max);
}



/*erosion : fix the of the pixels with the min of his neighbourds
param : list of pixels, index, width, height, format of the image
*/ 

Uint32 erosion(Uint32* pixels, int x, 
                    int width, int height, SDL_PixelFormat* format)
{		  
	int placeX = x / width;
	int placeY = x % width;
	
	Uint8 r, g, b;
	
	Uint8 min = 255;

	for (int i = - NEIGHBOURD; i <= NEIGHBOURD; i++)
	{
		for (int j = - NEIGHBOURD; j <= NEIGHBOURD; j++)
		{
			if (!(i == 0 && j == 0) &&
			    placeX + i >= 0 && placeX + i < height &&
				placeY + j >= 0 && placeY + j < width)
			{
				SDL_GetRGB(
                    pixels[((placeX+i)*width)+placeY+j], format, &r, &g, &b);
				min = (min > r) ? r : min;
			}
		}
	}
	
	//SDL_GetRGB(pixels[x], format, &r, &g, &b);
	//min = min+r;
	
	return SDL_MapRGB(format, min, min, min);
}



/*surface_to_binarization : convert the image in just black and white
param : surface, the surface of the image
*/

void surface_to_dilatation_and_erosion(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;

    int len = surface->w * surface->h;

    int width = surface->w;
    
    int height = surface->h;

	Uint32* copy_pixels;
	
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);
    
    copy_pixels = (Uint32*) malloc(len*sizeof(Uint32));
    //deep copy of pixels into copy_pixels
    memcpy(copy_pixels, pixels, len*sizeof(Uint32));
	
    for (int i = 0; i < len; i++)
    {
		pixels[i] = dilatation(copy_pixels, i, width, height, format);
	}
	
	
	free(copy_pixels);
	copy_pixels = (Uint32*) malloc(len*sizeof(Uint32));
    //deep copy of pixels into copy_pixels
    memcpy(copy_pixels, pixels, len*sizeof(Uint32));
    
	 
	for (int i = 0; i < len; i++)
    {
		pixels[i] = erosion(copy_pixels, i, width, height, format);
	}

	free(copy_pixels);
	
    SDL_UnlockSurface(surface);
}

void surface_to_dilatation(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;

    int len = surface->w * surface->h;

    int width = surface->w;
    
    int height = surface->h;

	Uint32* copy_pixels;
	
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);
    
    copy_pixels = (Uint32*) malloc(len*sizeof(Uint32));
    //deep copy of pixels into copy_pixels
    memcpy(copy_pixels, pixels, len*sizeof(Uint32));
	
    for (int i = 0; i < len; i++)
    {
		pixels[i] = dilatation(copy_pixels, i, width, height, format);
	}
	
	free(copy_pixels);
	
    SDL_UnlockSurface(surface);
}

void surface_to_erosion(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;

    int len = surface->w * surface->h;

    int width = surface->w;
    
    int height = surface->h;

	Uint32* copy_pixels;
	
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);
    
    copy_pixels = (Uint32*) malloc(len*sizeof(Uint32));
    //deep copy of pixels into copy_pixels
    memcpy(copy_pixels, pixels, len*sizeof(Uint32));
	
    for (int i = 0; i < len; i++)
    {
		pixels[i] = erosion(copy_pixels, i, width, height, format);
	}
	
	free(copy_pixels);
	
    SDL_UnlockSurface(surface);
}
