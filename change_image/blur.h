#ifndef NEIGHBOURD

#define NEIGHBOURD 1

#endif

#ifndef BLUR_H
#define BLUR_H

//blur all the pixels of the surface

Uint32 pixel_to_blur(Uint32* pixels, int x, 
              int width, int height, SDL_PixelFormat* format, int kernel[9]);
 
/*surface_to_blur : change the image to blur image with gaussian filter
param : surface, the surface of the image
*/

void surface_to_blur(SDL_Surface* surface);

#endif
