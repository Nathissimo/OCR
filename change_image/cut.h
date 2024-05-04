#pragma once

#define DEBUG_CUT 0

SDL_Surface** cut_image_into_squares(SDL_Surface* image, int squares_per_row);

void remove_lines(float* image, int w, int h);

void displayImageAscii(float *image, int width, int height);

float *getPixelValues(SDL_Surface *surface);
