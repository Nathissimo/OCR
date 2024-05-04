#ifndef BINARIZATION_H
#define BINARIZATION_H

/*pixel_to_binarization : convert the pixel into black or white
*/

Uint32 pixel_to_binarization(Uint32 pixel_color, 
        SDL_PixelFormat* format, Uint8 threshold);

/*surface_to_binarization : convert the image in just black and white
param : surface, the surface of the image
*/

void surface_to_binarization(SDL_Surface* surface);

#endif
