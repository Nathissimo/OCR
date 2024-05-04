#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>
#include "change_image/blur.h"
#include "change_image/binarization.h"
#include "change_image/dilatation_and_erosion.h"
#include "change_image/grayscale.h"
#include "change_image/sobel.h"
#include "change_image/rotation.h"
#include "change_image/detection.h"
#include "change_image/draw_grid.h"
#include "solve/solver.h"
#include "change_image/rescale.h"
#include "change_image/cut.h"
#include "Network/network.h"
#include "Network/training.h"


// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);

    SDL_Surface* surface_rvb = 
        SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888,0);
    if (surface_rvb == NULL)
	    errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_FreeSurface(surface);

    return surface_rvb;
}

int main_ocr(char* filename)
{
    // - Create a surface from the colored image.
    SDL_Surface* temp_surface = load_image(filename);
    if (temp_surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    size_t neww;
    size_t newh;
    size_t min_size = 1000;
    if (temp_surface->w > temp_surface->h) {
        newh = min_size;
        neww = min_size*temp_surface->w/temp_surface->h;
    }
    else {
        neww = min_size;
        newh = min_size*temp_surface->h/temp_surface->w;
    }
    //printf("w%ld  h%ld\n", neww, newh);
    SDL_Surface *surface = resize(temp_surface, neww, newh);
    if (surface == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

	SDL_SaveBMP(surface, "temp_files/colored.png");

    // - Convert the surface into grayscale.
    
    surface_to_grayscale(surface);

    SDL_SaveBMP(surface, "temp_files/grayscale.png");
        
    // - Convert the surface into blur.
    
    for (int i = 0; i < 5; i++)
        surface_to_blur(surface);
    
    SDL_SaveBMP(surface, "temp_files/blur.png");

    // - Convert the surface into dilatation and erosion (dilero).
    
    surface_to_dilatation_and_erosion(surface);
    
    SDL_SaveBMP(surface, "temp_files/dilero.png");

       
    // - Convert the surface into sobel.
    
    surface_to_sobel(surface);
    
    SDL_SaveBMP(surface, "temp_files/sobel.png");

    // - Convert the surface into binarization.
    
    surface_to_binarization(surface);

    SDL_SaveBMP(surface, "temp_files/binarization.png");

    // - Convert the surface into better analyse.
       
    surface_to_dilatation(surface);
    surface_to_dilatation(surface);
    surface_to_dilatation(surface);

    SDL_SaveBMP(surface, "temp_files/analyse.png");

    // - Convert the surface into detection
    
    
    size_t* nsewe = get_sudoku(surface);

    SDL_SaveBMP(surface, "temp_files/lines.png");

    // - Load surface from binarization image

    /* SDL_Surface* surface_binarization = load_image("temp_files/binarization.png");
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError()); */
        
    // - Create sudoku surface from binarization
    SDL_Surface* surface_blur = load_image("temp_files/blur.png");
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Surface* sudoku = 
        create_image(surface_blur, nsewe[3], nsewe[0], nsewe[2], nsewe[1]);
//-------------------------------------------
    surface_to_dilatation(sudoku);
    surface_to_sobel(sudoku);
    surface_to_dilatation(sudoku);
    surface_to_erosion(sudoku);
    surface_to_dilatation(sudoku);
    surface_to_erosion(sudoku);
    surface_to_erosion(sudoku);
    surface_to_binarization(surface);
    surface_to_binarization(surface);

    IMG_SavePNG(sudoku, "temp_files/sudoku.png");
        
    sudoku = resize(sudoku, 252, 252);
    
	// - Cut the sudoku
	
    SDL_Surface** images = cut_image_into_squares(sudoku,9);

    int grid[9][9] = {};
    Network *network = load_network(
        "detect_digit.network");
    //print_neurons(10, network->n_layers[1]);
    assert(network != NULL);
    for (int i = 0; i < 81; i++)
    {
	    char name[100];
	    sprintf(name,"temp_files/number_%i.png",i);
	    SDL_SaveBMP(images[i], name);
        grid[i % 9][i / 9] = 
            get_digit_in_surface(name, network);
        //printf("%d ",grid[i % 9][i / 9]);
        free(images[i]);
    }
    //print_neurons(20, network->n_layers[1]);
    free(images);
    free_network(network);
    // - Load surface from colored image

    SDL_Surface* surface_colored = load_image("temp_files/colored.png");
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Convert the surface with character in the grid.

	int result[9][9] = {};

	main_solve(grid,result);

    draw_all_digit(surface_colored, nsewe[3]+10,nsewe[0]+10,nsewe[2]-nsewe[3],
            nsewe[1]-nsewe[0]-20, grid, result);
    
	SDL_SaveBMP(surface_colored, "sudoku_result.png");
    
    // - Free the surface.
    
    SDL_FreeSurface(surface);

    return EXIT_SUCCESS;
}
