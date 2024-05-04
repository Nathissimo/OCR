#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "dilatation_and_erosion.h"
#include <math.h>

/*sobel : fix the value of the pixels with the vertical and the horizontal 
 * matrix
param : list of pixels, index, width, height, format of the image
*/ 

Uint32 sobel(Uint32* pixels, int x, int width, int height, 
	     int vertical[9], int horizontal[9], SDL_PixelFormat* format)
{
	int placeX = x / width;
	int placeY = x % width;
	
	Uint8 r, g, b;
	
	int valueX_red = 0;
	int valueX_green = 0;
	int valueX_blue = 0;
	
	int valueY_red = 0;
	int valueY_green = 0;
	int valueY_blue = 0;
	
	int value_red = 0;
	int value_green = 0;
	int value_blue = 0;

	int place = 0;
	
	for (int i = - NEIGHBOURD; i <= NEIGHBOURD; i++)
	{
		for (int j = - NEIGHBOURD; j <= NEIGHBOURD; j++)
		{
			if (placeX + i >= 0 && placeX + i < height &&
				placeY + j >= 0 && placeY + j < width)
			{
				SDL_GetRGB(
                    pixels[((placeX+i)*width)+placeY+j], format, &r, &g, &b);
               
					
				valueX_red += r*vertical[place];
				valueY_red += r*horizontal[place];
				valueX_green += g*vertical[place];
				valueY_green += g*horizontal[place];
				valueX_blue += b*vertical[place];
				valueY_blue += b*horizontal[place];
				place++;
			}
		}
	}
	
	value_red = sqrt(valueX_red*valueX_red + valueY_red*valueY_red);
	value_green = sqrt(valueX_green*valueX_green + valueY_green*valueY_green);
	value_blue = sqrt(valueX_blue*valueX_blue + valueY_blue*valueY_blue);
	
	if (value_red > 255)
		value_red = 255;
	
	if (value_green > 255)
		value_green = 255;
	
	if (value_blue > 255)
		value_blue = 255;
	
	return SDL_MapRGB(format, value_red, value_green, value_blue);
}



/*surface_to_sobel : convert the image to display more the line
param : surface, the surface of the image
*/

void surface_to_sobel(SDL_Surface* surface)
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
	
	int horizontal[9] = {-1, -2, -1,
					    0,  0,  0,
					    1,  2,  1};
					   
	int vertical[9] = {-1, 0, 1,
						 -2, 0, 2,
						 -1, 0, 1};
	
    for (int i = 0; i < len; i++)
    {
	    pixels[i] = 
            sobel(copy_pixels, i, width, height, vertical, horizontal, format);
	}
	
	free(copy_pixels);
	
    SDL_UnlockSurface(surface);    
}
