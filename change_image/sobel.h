#ifndef NEIGHBOURD

#define NEIGHBOURD 1

#endif
#ifndef SOBEL_H
#define SOBEL_H

/*sobel : fix the value of the pixels with the vertical and the horizontal matrix
param : list of pixels, index, width, height, format of the image and the matrix
*/ 

Uint32 sobel(Uint32* pixels, int x, int width, int height, 
	     int vertical[9], int horizontal[9], SDL_PixelFormat* format);
                    
/*surface_to_sobel : convert the image to display more the line
param : surface, the surface of the image
*/

void surface_to_sobel(SDL_Surface* surface);

#endif
