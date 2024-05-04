#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "blur.h"

/*pixel_to_blur: convert the pixel with the kernel matrix and his neighbour
*/

Uint32 pixel_to_blur(Uint32* pixels, int x, 
              int width, int height, SDL_PixelFormat* format, int KERNEL9[9])
{
	int placeX = x / width;
	int placeY = x % width;
	
	Uint8 r, g, b;

	int Sr = 0;
	int Sg = 0;
	int Sb = 0;
	
	int power = 0;
		
	for (int i = - NEIGHBOURD; i <= NEIGHBOURD; i++)
	{
		for (int j = - NEIGHBOURD; j <= NEIGHBOURD; j++)
		{
			if (placeX + i >= 0 && placeX + i < height &&
				placeY + j >= 0 && placeY + j < width)
			{

				SDL_GetRGB(pixels[((placeX+i)*width)+placeY+j], 
		                       format, &r, &g, &b);

 				Sr += r*KERNEL9[(NEIGHBOURD+i)*3+NEIGHBOURD+j];
 				Sg += g*KERNEL9[(NEIGHBOURD+i)*3+NEIGHBOURD+j];
				Sb += b*KERNEL9[(NEIGHBOURD+i)*3+NEIGHBOURD+j];
				power += KERNEL9[(NEIGHBOURD+i)*3+NEIGHBOURD+j];
			}
		}
	}

	return SDL_MapRGB(format,Sr / power,Sg / power,Sb / power);
}



/*surface_to_blur : change the image to blur image with gaussian filter
param : surface, the surface of the image
*/

void surface_to_blur(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;

	Uint32* copy_pixels = surface->pixels;
	
    int len = surface->w * surface->h;
    
    int width = surface -> w;
    
    int height = surface -> h;

    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);
    
    int KERNEL9[9] = 
	{ 59, 97, 59,
	  97, 159, 97,
	  59, 97, 59};
  
    for (int i = 0; i < 1; i++)
    {
		for (int i = 0; i < len; i++)
		{
    pixels[i] = pixel_to_blur(copy_pixels, i, width, height, format, KERNEL9);
		}
	}

    SDL_UnlockSurface(surface);    
}
