#include <err.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <assert.h>
#include "cut.h"


//Cut the surface into squares_per_pow surface²
SDL_Surface** cut_image_into_squares(SDL_Surface* image, int squares_per_row) {
    //Size of every surfaces
    int surface_width = image->w / squares_per_row;
    int surface_height = image->h / squares_per_row;
    
    //array of new surfaces
    SDL_Surface** surfaces = (SDL_Surface**)malloc(
        squares_per_row * squares_per_row * sizeof(SDL_Surface*));
    
    //cut in surface
    for (int i = 0; i < squares_per_row; i++) {
        for (int j = 0; j < squares_per_row; j++) {
            //rect of the surface
            SDL_Rect rect;
            rect.x = i * surface_width;
            rect.y = j * surface_height;
            rect.w = surface_width;
            rect.h = surface_height;
            
            //take surface
            SDL_Surface* surface = SDL_CreateRGBSurface(
                0, surface_width, surface_height, 32, 0,0,0,0);
            SDL_BlitSurface(image, &rect, surface, NULL);
            
            //save in the array
            surfaces[i * squares_per_row + j] = surface;
        }
    }
    
    return surfaces;
}

/*
This function takes in a one-dimensional float array representing an image, 
a horizontal size, and a vertical size as inputs. 
The function will set to 0 all the values of all the lines that contain more 
than wmax different values of 0. It will also set to 0 all the values of all 
the columns that contain more than hmax different values of 0. 
Nothing is returned by the function.
*/
void remove_lines(float* image, int w, int h)
{
    const float w_threshold = 0.70;
    const float h_threshold = 0.80;

    int wmax = (int)(w*w_threshold);
    int hmax = (int)(h*h_threshold);

    int line_count[h]; 
    //creates an array to count the number of 0s on each line
    int column_count[w]; 
    //creates an array to count the number of 0s on each column
    // Initialization of the counter array to 0
    for (int i = 0; i < h; i++)
        line_count[i] = 0;
    for (int i = 0; i < w; i++)
        column_count[i] = 0;

    //Count of the number of non 0s on each line
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (image[i * w + j] != 0)
            {
                line_count[i]++;
                column_count[j]++;
            }
        }
    }

    for (int i = 0; i < h/8; i++)
    {
        line_count[i] = wmax+1;
        line_count[h-i-1] = line_count[i];
    }
    for (int j = 0; j < w/5; j++)
    {
        column_count[j] = hmax+1;
        column_count[w-j-1] = column_count[j];
    }

    //Set to 0 the lines with more than wmax different values from 0
    for (int i = 0; i < h; i++)
    {
        if (line_count[i] > wmax)
        {
            for (int j = 0; j < w; j++)
            {
                image[i * w + j] = 0;
            }
        }
    }
    //Set to 0 the columns with more than hmax different values from 0
    for (int i = 0; i < w; i++)
    {
        if (column_count[i] > hmax)
        {
            for (int j = 0; j < h; j++)
            {
                image[j * w + i] = 0;
            }
        }
    }
}

// This function displays an image in ASCII art form.
// The image is passed as a one-dimensional float array, with 0.0s and 1.0s.
// The width and height of the image are passed as parameters as well.
void displayImageAscii(float *image, int width, int height)
{
    int x, y;
    char s0[] = " ";
    char s1[] = "*";
    char sjsp[] = "?";

    for (y = 0; y < height; y++)
    {
        printf("[");
        for (x = 0; x < width; x++)
        {
            //printf("%4.1f", image[y * width + x]);
            if (image[y * width + x] == 0.0)
                printf("%1s", s0);
            else if (image[y*width + x] == 1.0)
                printf("%1s", s1);
            else
                printf("%1s", sjsp);
        }
        printf("]\n");
    }
}

// Function that takes a surface and returns a float array
float* getPixelValues(SDL_Surface* surface)
{
    // Create a float array
    if (DEBUG_CUT>=2) printf("Create a float array\n");
    float* pixelValues = (float*)malloc(
        sizeof(float) * surface->w * surface->h);
    assert(pixelValues != NULL);

    // Get the surface data
    if (DEBUG_CUT>=2) printf("Get the surface data\n");
    Uint8* pixels = (Uint8*)surface->pixels;

    /* // For each pixel
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            // Get the pixel value
            if (DEBUG_CUT>=2) printf("Get the pixel value : index %d\n",
                y * surface->w + x);
            Uint32 pixel = pixels[y * surface->w + x];
            // Convert the pixel value to a float value
            if (DEBUG_CUT>=2) printf(
                "Convert the pixel value to a float value\n");
            //float value = (float)(pixel & 0xFF) / 255.0f;
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            float value = r > 250 ? 1.0 : 0.0;
            // Update the array
            if (DEBUG_CUT>=2) printf("Update the array\n");
            pixelValues[y * surface->w + x] = value;
        }
    } */
    for (int i = 0; i < surface->w*surface->h*3; i+=3)
    {
        // Get the pixel value
        if (DEBUG_CUT>=2) printf("Get the pixel value : %08x   index :%d\n",
            pixels[i], i);
        Uint8 r = pixels[i];
        Uint8 g = pixels[i+1];
        Uint8 b = pixels[i+2];
        // Convert the pixel value to a float value
        if (DEBUG_CUT>=2) printf(
            "Convert the pixel value to a float value\n");
        //float value = (float)(pixel & 0xFF) / 255.0f;
        /* Uint8 r, g, b, a;
        SDL_GetRGB(pixel, surface->format, &r, &g, &b, &a); */
        int tresh = 75;
        float value = r >= tresh || g >= tresh || b >= tresh ? 1.0 : 0.0;
        // Update the array
        if (DEBUG_CUT>=2) printf("Update the array\n");
        pixelValues[i/3] = value;
    }
    

    /* if (DEBUG_CUT>=1) printf("free surface\n");
    SDL_FreeSurface(surface); // free */

    //displayImageAscii(pixelValues, surface->w, surface->h);

    //remove horizontal and vertical lines
    remove_lines(pixelValues, surface->w, surface->h);

    //printf("\n\n");

    //displayImageAscii(pixelValues, surface->w, surface->h);

    /* char c;
    int tpmp = scanf("%c", &c);
    if (!c)
        printf("OH ZUT ! %d %d\n", c,tpmp); */

    // Return the float array
    if (DEBUG_CUT>=2) printf("Return the float array\n");
    return pixelValues;
}
