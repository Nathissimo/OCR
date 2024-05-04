#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "rescale.h"

SDL_Surface* resize(SDL_Surface* surface, size_t newx, size_t newy)
{
    SDL_Surface* newsurface= 
	    SDL_CreateRGBSurface(0,newx,newy,32,0,0,0,0);
    
    Uint32 *pixels = surface->pixels;
    size_t oldx = surface->w;
    size_t oldy = surface->h;
    Uint32 *newpixels = newsurface->pixels;

    float ratiox = (float)oldx / newx;
    float ratioy = (float)oldy / newy;
    //printf("x%f  y%f\n", ratiox, ratioy);

    for (size_t y = 0; y < newy; y++)
    {
        for (size_t x = 0; x < newx; x++)
        {
            newpixels[get_index(x,y,newx)] =
                pixels[get_index(x*ratiox,y*ratioy,oldx)];
        }
    }
    return newsurface;
}

size_t get_index(size_t x, size_t y, size_t maxx)
{
    return x + y * maxx;
}
