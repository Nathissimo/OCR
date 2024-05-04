#include <err.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <assert.h>
#include "network.h"
#include "training.h"
#include "../change_image/cut.h"
#include "../change_image/binarization.h"
#include "../change_image/grayscale.h"

/// @brief Get dataset from folders and files
/// @param foldername 
/// @param nb_data 
/// @param names 
/// @param num_max 
/// @param prefix 
/// @param suffix 
/// @return 
Dataset* get_datas(
    char foldername[], size_t nb_data, char* names[], size_t num_max,
    char prefix[], char suffix[])
{
    Dataset *dataset = (Dataset *)malloc(sizeof(Dataset));
    dataset->nb_data = nb_data;

    SDL_Surface *surface;

    // for each sign to recognize
    for (size_t img = 0; img < dataset->nb_data; img++)
    {
        dataset->data_tab[img] = (ImgTab)malloc(sizeof(Img) * num_max);
        assert(dataset->data_tab[img] != NULL);
        dataset->s_datas[img] = 0;
        // test all possibilities of naming
        for (size_t i = 0; i < num_max; i++) {
            char imgname[200];
            sprintf(imgname, "%s/%s/%s%ld%s", 
            foldername, names[img], prefix, i, suffix);
            surface = IMG_Load(imgname);
            if (surface != NULL)
            {
                /* for (size_t i = 0; i < 784; i++)
                {
                    if (i%28 == 0)
                        printf("\n");
                    printf("%08X ", ((Uint32*)(surface->pixels))[i]);
                }
                printf("\n"); */

                if (1+DEBUG >= 2)
                    printf("surface exist : \t%s\n", imgname);
                /* surface_to_grayscale(surface);
                surface_to_binarization(surface); */
                if (DEBUG >= 2)
                    printf("run getPixelValues(surface)\n");
                SDL_LockSurface(surface);
                Img tempimg = getPixelValues(surface);
                SDL_UnlockSurface(surface);
                if (DEBUG>=2) printf("save tempimg in dataset\n");
                dataset->data_tab[img][dataset->s_datas[img]] = tempimg; // save
                dataset->s_datas[img]++;
                if (DEBUG>=2) printf("free surface\n");
                SDL_FreeSurface(surface); // free
                surface = NULL; // reset
            }
            else
                if (DEBUG>=2) printf("surface == NULL : \t%s\n",imgname);
        }
        if (DEBUG>=2) printf("nb of %s detected : %ld\n",
            names[img],dataset->s_datas[img]);
        
        if (DEBUG>=2)
            printf("realloc(dataset->data_tab[%ld], %ld)\n",
            img,dataset->s_datas[img]);
        
        dataset->data_tab[img] = (ImgTab)realloc(
            dataset->data_tab[img], sizeof(Img) *dataset->s_datas[img]);
        assert(dataset->data_tab[img] != NULL);
    }

    return dataset;
}



/// @brief Free all structure.
/// @param dataset the dataset to free.
void free_dataset(Dataset* dataset)
{
    for (size_t img = 0; img < dataset->nb_data; img++) {
        for (size_t i = 0; i < dataset->s_datas[img]; i++) {
            char str[50];
            sprintf(str, "data_tab[%ld][%ld]", img, i);
            print_free(dataset->data_tab[img][i], str);
        }
        char str[50];
        sprintf(str, "data_tab[%ld]", img);
        print_free(dataset->data_tab[img], str);
    }
    print_free(dataset,"dataset");
}


/// @brief train 3 layer's network
void training(
    Network* network, char foldername[], char* names[],
    size_t num_max, size_t iterations, 
    char prefix[], char suffix[])
{
    //const int nbin   = (int) network->s_layers[0];
    //const int nbhide = (int) network->s_layers[1];
    const int nbout  = (int) network->s_layers[2];

    Dataset *dataset = 
        get_datas(foldername, nbout,names,num_max,prefix,suffix);

    //if (1) return;

    if (DEBUG >= 2) print_s_layers(dataset->nb_data, dataset->s_datas);

    if (DEBUG >= 1) printf("%ld\n",iterations);

    int nbdebug = ((iterations/1000)==0?1:(iterations/1000));

    InOut *calc_out;
    for (size_t cpt = 0; cpt < iterations; cpt++) //1056
    {
        if (DEBUG>=2) printf("\n- - - - -prop- - - - -\n");

        
        if (DEBUG>=2) printf("rand choices\n");
        int num_choice = rand() % nbout; // which symbol
        // which image of this symbol
        int img_choice = rand() % dataset->s_datas[num_choice];

        //-----------------------------------------
        if (DEBUG>=2) printf("init wanted out\n");
        InOut wanted_out = (InOut)malloc(sizeof(float) * nbout);
        for (int i = 0; i < nbout; i++) {
            wanted_out[i] = num_choice == i ? 1.0 : 0.0;
        }
        if (DEBUG>=2) printf("run prop_network\n");
        /* InOut*  */ calc_out = prop_network(
            network, dataset->data_tab[num_choice][img_choice]);
        
        

        if (DEBUG>=2)
            for (size_t l = 0; l < network->nb_layers; l++) {
                print_inout(network->s_layers[l], calc_out[l], "calc_out[l]");
            }
        
        if (DEBUG>=2) printf("\n- - - - -before back prop- - - - -\n");
        if (DEBUG>=2) print_network(network);
        //back prop
        if (DEBUG>=2) printf("\n- - - - -back prop- - - - -\n");
        float error_delta = back_prop_network(network, calc_out, wanted_out);
        if (DEBUG>=1 && cpt%(nbdebug*10)==0) {
            printf("iteration:%8ld  err_d:%9f",cpt, error_delta);
            printf("  w:%.0f", wanted_out[0]);
            for (int i = 1; i < nbout; i++){
                printf(",%.0f", wanted_out[i]);
            }
            printf("  c:%.2f", calc_out[network->nb_layers-1][0]);
            for (int i = 1; i < nbout; i++){
                printf(",%.2f", calc_out[network->nb_layers-1][i]);
            }
            for (float i = 0.1; i < fabs(error_delta)*400; i++) {
                printf("-");
            }
            printf("\n");
        }
        /* if (DEBUG>=1 && cpt%(nbdebug*1000) == 0){
            save_network(network, "temp_28x28_0123456789_im1and2and3");
        } */
        if (DEBUG>=2) printf("- - - - -after back prop- - - - -\n");
        if (DEBUG>=2) print_network(network);
        if (DEBUG>=2) printf("- - - - -   - - - - -\n");

        //free section
        print_free(wanted_out, "wanted_out");
        for (size_t l = 1; l < network->nb_layers; l++) {
            char str[50];
            sprintf(str, "calc_out[%ld]", l);
            print_free(calc_out[l], str);
        }
        print_free(calc_out, "calc_out");
    }
    free_dataset(dataset);
    if (DEBUG >= 2)
        print_network(network);
    save_network(network, "28x28_0123456789_im1and2and3");
    //free_network(network);
}


int get_digit_in_surface(char img_name[], Network *network)
{
    
    SDL_Surface* surface = IMG_Load(img_name);
    assert(surface != NULL);
    InOut pixels = getPixelValues(surface);
    assert(pixels != NULL);
    //print_inout(784, pixels, "pixels");
    InOut* out = prop_network(network, pixels);
    //print_inout(10,out[2],"out");
    SDL_FreeSurface(surface);
    float max_prop = 0.0;
    int max_prop_digit = 0;
    for (int i = 0; i < 10; i++)
    {
        if (out[2][i]>max_prop) {
            max_prop = out[2][i];
            max_prop_digit = i;
        }
    }
    print_free(out[0], "out[0]");
    print_free(out[1], "out[1]");
    print_free(out[2], "out[2]");
    print_free(out, "out");
    return max_prop_digit;
}
