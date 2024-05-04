#ifndef GRAYSCALE_H
#define GRAYSCALE_H

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface);

#endif
