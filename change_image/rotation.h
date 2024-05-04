#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

#ifndef ROTATION_H
#define ROTATION_H


Uint32 pixel_to_black(Uint32 pixel_color, SDL_PixelFormat* format);
void draw(SDL_Renderer* renderer, SDL_Texture* texture);

Uint32 copy_pixel(Uint32 pixel,SDL_PixelFormat* format);
long to_int(float fl);
SDL_Surface* sub_pixel(SDL_Surface* surface,int sub);
SDL_Surface* mul_pixel(SDL_Surface* surface,int mul);
void surface_to_turn(SDL_Surface* surface, float val_angle);
void surface_to_turn_an(SDL_Surface* surface, float val_angle);
#endif
