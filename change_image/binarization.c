#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "binarization.h"

/*pixel_to_binarization : convert the pixel into black or white
*/

Uint32 pixel_to_binarization(Uint32 pixel_color, 
        SDL_PixelFormat* format, Uint8 threshold)
{
	Uint8 r, g, b;
	SDL_GetRGB(pixel_color, format, &r, &g, &b);
	
	if (r < threshold)
		return SDL_MapRGB(format, 0, 0, 0);
	
	return SDL_MapRGB(format, 255, 255, 255);
}


/*surface_to_binarization : convert the image in just black and white
param : surface, the surface of the image
*/

void surface_to_binarization(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
	
    int len = surface->w * surface->h;
    
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

	int histData[256] = {};
	Uint8 r, g, b;
	// Calculate histogram
	for (int i = 0; i < len; i++)
	{
	   SDL_GetRGB(pixels[i], format, &r, &g, &b);
	   histData[r]++;
	}
		
	float sum = 0;
	for (int t=0 ; t<256 ; t++) 
		sum += t * histData[t];

	float sumB = 0;
	int wB = 0;
	int wF = 0;

	float varMax = 0;
	Uint8 threshold = 0;

	for (int t=0 ; t<256 ; t++) 
	{
	   wB += histData[t];               // Weight Background
	   if (wB == 0) continue;

	   wF = len - wB;                 // Weight Foreground
	   if (wF == 0) break;

	   sumB += (float) (t * histData[t]);

	   float mB = sumB / wB;            // Mean Background
	   float mF = (sum - sumB) / wF;    // Mean Foreground

	   // Calculate Between Class Variance
	   float varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);

	   // Check if new maximum found
	   if (varBetween > varMax) 
	   {
		  varMax = varBetween;
		  threshold = t;
	   }
	}
	
    for (int i = 0; i < len; i++)
    {
		pixels[i] = pixel_to_binarization(pixels[i], format, threshold);
	}
    SDL_UnlockSurface(surface);    
}
