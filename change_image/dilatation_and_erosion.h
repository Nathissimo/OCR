#ifndef NEIGHBOURD

#define NEIGHBOURD 1

#endif
#ifndef DILATATION_AND_EROSION_H
#define DILATATION_AND_EROSION_H

/*dilatation : fix the of the pixels with the max of his neighbourds
param : list of pixels, index, width, height, format of the image
*/ 

Uint32 dilatation(Uint32* pixels, int x, 
                    int width, int height, SDL_PixelFormat* format);
                    
/*erosion : fix the of the pixels with the min of his neighbourds
param : list of pixels, index, width, height, format of the image
*/ 

Uint32 erosion(Uint32* pixels, int x, 
                    int width, int height, SDL_PixelFormat* format);
                    
/*surface_to_binarization : convert the image in just black and white
param : surface, the surface of the image
*/

void surface_to_dilatation_and_erosion(SDL_Surface* surface);

void surface_to_dilatation(SDL_Surface* surface);
void surface_to_erosion(SDL_Surface *surface);

#endif
