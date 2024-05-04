#ifndef TRANING_H
#define TRANING_H

#include <stdlib.h>
#include "network.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//numbers 0 to 9 included
#define MAX_NUM 10

// -----------------------------------

/// @brief Img is a float's tab of images in black or white (0.0 or 1.0)
typedef float* Img;

/// @brief ImgTab is a float's tab of images tab
typedef Img* ImgTab;

/// @brief A dataset with images in black or white (0.0 or 1.0)
typedef struct
{
    size_t nb_data;
    size_t s_datas[MAX_NUM];
    ImgTab data_tab[MAX_NUM];
}Dataset;


// -----------------------------------

/// @brief Get dataset from folders and files
/// @param foldername 
/// @param nb_data 
/// @param names 
/// @param num_max 
Dataset* get_datas(
    char foldername[], size_t nb_data, char* names[], size_t num_max,
    char prefix[], char suffix[]);


/// @brief train 3 layer's network
void training(
    Network* network, char foldername[], char* names[],
    size_t num_max, size_t iterations, 
    char prefix[], char suffix[]);

int get_digit_in_surface(char img_name[], Network* network);

#endif