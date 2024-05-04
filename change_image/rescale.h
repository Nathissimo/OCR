#ifndef RESCALE_H
#define RESCALE_H

//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL.h>

SDL_Surface *resize(SDL_Surface *surface, size_t newx, size_t newy);
size_t get_index(size_t x, size_t y, size_t maxx);

#endif
