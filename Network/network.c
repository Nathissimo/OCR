#include <err.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "network.h"
#include "training.h"


Neuron create_neuron(Network* network, size_t l_index, size_t n_index)
{
    Neuron neuron;

    //index_prev
    neuron.wl_index_prev = l_index-1;
    if (l_index>0) {
        neuron.w_index_prev = (size_t*) malloc(
            network->s_layers[l_index-1] * sizeof(size_t));
        assert(neuron.w_index_prev != NULL);
        for (size_t i = 0; i<network->s_layers[l_index-1]; i++) {
            neuron.w_index_prev[i] = i*network->s_layers[l_index] + n_index;
            if (DEBUG>=3)
                printf(
                    "layer:%ld\tne.w_idx_prev[%ld] = %ld*%ld + %ld = %ld\n",
                    l_index-1,i,i,network->s_layers[l_index],n_index,
                    neuron.w_index_prev[i]);
        }
    }
    else
        neuron.w_index_prev = NULL;

    neuron.bias = 0.0;
    neuron.delta = 0.0;

    //index_next
    neuron.wl_index_next = l_index;
    if (l_index<network->nb_layers-1) {
        
        neuron.w_index_next = (size_t*) malloc(
            network->s_layers[l_index] * sizeof(size_t));
        assert(neuron.w_index_next != NULL);
        for (size_t i = 0; i<network->s_layers[l_index+1]; i++) {
            neuron.w_index_next[i] = i + network->s_layers[l_index+1]*n_index;
            if (DEBUG>=3)
                printf(
                    "layer:%ld\tne.w_idx_next[%ld] = %ld*%ld + %ld = %ld\n",
                    l_index,i,i,network->s_layers[l_index+1],n_index,
                    neuron.w_index_next[i]);
        }
    }
    else
        neuron.w_index_next = NULL;

    return neuron;
}


Layer create_neuron_layer(Network* network, size_t index)
{
    // warning: ‘*network.s_layers[0]’ may be used uninitialized !?
    Layer layer = (Layer) malloc(network->s_layers[index] * sizeof(Neuron));
    assert(layer != NULL);

    for (size_t i = 0; i<network->s_layers[index]; i++) {
        layer[i] = create_neuron(network, index, i);
    }

    return layer;
}


Network* create_network(size_t nb_layers, size_t layers_sizes[])
{
    Network* network = (Network*) malloc(1*sizeof(Network));
    assert(network != NULL);
    network->nb_layers = nb_layers;

    for (size_t i = 0; i<network->nb_layers; i++) {
        network->s_layers[i] = layers_sizes[i];
    }

    network->n_layers[0] = create_neuron_layer(network, 0);

    for (size_t i = 1; i<network->nb_layers; i++) {        
        network->w_layers[i-1] = (Weights) malloc(
            network->s_layers[i-1] * network->s_layers[i] * sizeof(float));
        assert(network->w_layers[i-1] != NULL);
        network->n_layers[i] = create_neuron_layer(network, i);
    }

    return network;
}



void free_network(Network* network)
{
    //free neurons
    for (size_t l = 0; l<network->nb_layers; l++) {
        if (l<network->nb_layers-1)
            free(network->w_layers[l]);
        for (size_t n = 0; n<network->s_layers[l]; n++) {
            if (l>0) {
                if (network->n_layers[l][n].w_index_prev != NULL)
                    free(network->n_layers[l][n].w_index_prev);
            }
            if (l<network->nb_layers-1) {
                if (network->n_layers[l][n].w_index_prev != NULL)
                    free(network->n_layers[l][n].w_index_next);
            }
        }
        free(network->n_layers[l]);
    }
    free(network);
}



void init_weights(Network* network)
{
    for (size_t i = 0; i<network->nb_layers-1; i++) {
        size_t len_weights = network->s_layers[i] * network->s_layers[i+1];
        for (size_t j = 0; j<len_weights; j++) {
            network->w_layers[i][j] = 
                randsign()*(0.2+ ((float)rand() / (float)RAND_MAX)* 0.2);
            //printf("rand:%f\n", network->w_layers[i][j]);
            // 0.3;
        }
    }
}

void init_bias(Network* network)
{
    for (size_t i = 0; i<network->nb_layers; i++) {
        for (size_t j = 0; j<network->s_layers[i]; j++) {
            network->n_layers[i][j].bias = 
                randsign()*
                ((float)rand() / (float)RAND_MAX);
        }
    }
}

Network* init_network(Network* network)
{
    init_weights(network);
    init_bias(network);
    return network; // maybe remove it later
}


Network* load_network(char filename[])
{
    FILE* fileAddress;
	fileAddress = fopen(filename, "r");

	if (fileAddress == NULL)
    	errx(1, "Error loading file %s ", filename);

    size_t nb_layers;
    size_t s_layers[MAX_LAYERS];

    if(fscanf(fileAddress, "%ld", &nb_layers)==EOF)
        errx(1, "Can't read nb_layers.");
    //printf("nb_layers:%ld ", nb_layers);
    for (size_t l = 0; l < nb_layers; l++)
    {
        if(fscanf(fileAddress, "%ld", &(s_layers[l]))==EOF)
            errx(1, "Can't read s_layers[%ld].",l);
        //printf("s_layers[l]:%ld ", s_layers[l]);
    }
    Network* network = create_network(nb_layers, s_layers);

    for (size_t l = 0; l<nb_layers-1; l++) {
        for (size_t b = 0; b<s_layers[l]; b++) {
            float tempbias;
            int nbread;
            nbread = fscanf(fileAddress, "%f", &(tempbias));
            if (nbread == EOF)
                errx(1, "Can't read n_layers[%ld][%ld].",l,b);
            else
                network->n_layers[l][b].bias = tempbias;
            //printf("%dread n_layers[%ld][%ld] = %f\t",
                   //nbread, l, b, tempbias);
        }
        size_t max_w = s_layers[l]*s_layers[l+1];
        for (size_t w = 0; w<max_w; w++)
        {
            if(fscanf(fileAddress, "%f", &(network->w_layers[l][w]))==EOF)
                errx(1, "Can't read w_layers[%ld][%ld].",l,w);
        }
    }

    for (size_t b = 0; b<s_layers[nb_layers-1]; b++) {
        if(fscanf(fileAddress, "%f",
                    &(network->n_layers[nb_layers-1][b].bias))==EOF)
            errx(1, "Can't read n_layers[%ld][%ld].",nb_layers-1,b);
    }

    fclose(fileAddress);
    return network;
}

void save_network(Network* network, char filename[])
{
    char newfilename[100];
    sprintf(newfilename, "Network/%s.network", filename);

    FILE *f;
    f = fopen(newfilename, "w");
    
    if (f == NULL)
    	errx(1, "Error creating file %s", newfilename);


    fprintf(f,"%ld\n",network->nb_layers);
    // - - - - - - - - - - - - - - - - - - - - - - - - - -
    fprintf(f,"%ld",network->s_layers[0]);
    for (size_t i = 1; i<network->nb_layers; i++) {
        fprintf(f," %ld",network->s_layers[i]);
    }
    fprintf(f,"\n");
    // - - - - - - - - - - - - - - - - - - - - - - - - - -
    for (size_t l = 0; l<network->nb_layers-1; l++) {
        // - - - - - - - - - - - - - - - - - - - - - - - - - -
        fprintf(f,"%f",network->n_layers[l][0].bias);
        for (size_t i = 1; i<network->s_layers[l]; i++) {
            fprintf(f," %f",network->n_layers[l][i].bias);
        }
        fprintf(f,"\n");
        // - - - - - - - - - - - - - - - - - - - - - - - - - -
        fprintf(f,"%f",network->w_layers[l][0]);
        for (size_t i = 1; i<network->s_layers[l]*network->s_layers[l+1]; i++) 
        {
            fprintf(f," %f",network->w_layers[l][i]);
        }
        fprintf(f,"\n");
    }
    fprintf(f,"%f",network->n_layers[network->nb_layers-1][0].bias);
    for (size_t i = 1; i<network->s_layers[network->nb_layers-1]; i++) {
        fprintf(f," %f",network->n_layers[network->nb_layers-1][i].bias);
    }
    fprintf(f,"\n");
	// - - - - - - - - - - - - - - - - - - - - - - - - - -
	fclose(f);
}


// to calculate sigmoid
float sigmoid(float x)
{
	return (float)(1.0/(1.0+exp(-x)));
}

// to calculate prime sigmoid
float sigmoid_prime(float x)
{
	return sigmoid(x) * (1.0 - sigmoid(x));
}

// return 1 or -1 randomly
int randsign()
{
    return 2*(rand() % 2)-1;
}

/* - - - - - - - - - - - - - - - - - - - - - - */

/// @brief Calculate the prop activation of the neuron given.
/// @param network The neural network pointer.
/// @param neuron The neuron which output is calculated.
/// @param inputs The inputs of the neuron.
/// @return The output value of the neuron.
float prop_neuron(Network* network, Neuron* neuron, InOut inputs)
{
    //print_neuron(network, neuron);
    float sum = 0.0;
    for (size_t i = 0; i<network->s_layers[neuron->wl_index_prev]; i++) {
        /* printf(
            "%f * %f\twl prev:%lu \tw prev[%ld]%lu\tslayers[%ld]:%ld\tslayers\
[%ld]:%ld\n", 
        network->w_layers[neuron->wl_index_prev][neuron->w_index_prev[i]],
        inputs[i], // %f * %f
        neuron->wl_index_prev, //wl prev:%lu
        i, neuron->w_index_prev[i], //w prev[%ld]%lu
        //slayers[%ld]:%ld
        neuron->wl_index_prev, network->s_layers[neuron->wl_index_prev],
        //slayers[%ld]:%ld
        neuron->wl_index_prev+1, network->s_layers[neuron->wl_index_prev+1]); 
        */
        /* printf("%f * %f\t\n",
            network->w_layers[neuron->wl_index_prev][neuron->w_index_prev[i]],
            inputs[i]); // %f * %f
        printf("wl prev:%lu \t\n",neuron->wl_index_prev);
        printf("w prev[%ld]%lu\t\n",i, neuron->w_index_prev[i]);
        printf("slayers[%ld]:%ld\t\n",neuron->wl_index_prev, 
                               network->s_layers[neuron->wl_index_prev]);
        printf("slayers[%ld]:%ld\n",neuron->wl_index_prev+1, 
                           network->s_layers[neuron->wl_index_prev+1]); */

        //printf("wl prev:%ld \t",neuron->wl_index_prev);
        //printf("w prev[%ld]%ld\t\n",i, neuron->w_index_prev[i]);

        /* size_t max = network->s_layers[neuron->wl_index_prev]
            * network->s_layers[neuron->wl_index_next];
        if (neuron->w_index_prev[i] > max) {
            size_t slwlidprev = network->s_layers[neuron->wl_index_prev];
            for (size_t j = i; j < slwlidprev; j++) {
                printf("w prev[%ld]%lu\t(debug)\n",j, neuron->w_index_prev[i]);
            }
        } */
        //print_network(network);
        sum +=
 network->w_layers[neuron->wl_index_prev][neuron->w_index_prev[i]] * inputs[i];
    }
    sum += neuron->bias;
    return sigmoid(sum);
}



/// @brief Apply the prop on all neurons exept in input layer.
/// @param network The neural network pointer.
/// @return output of each neuron layer by layer.
InOut* prop_network(Network* network, InOut inputs)
{
    InOut* output = (InOut*)malloc(network->nb_layers*sizeof(InOut));
    assert(output != NULL);
    output[0] = inputs;
    for (size_t l = 1; l < network->nb_layers; l++)
    {
        output[l] = layer_malloc(network,l);
        assert(output[l] != NULL);
        for (size_t n = 0; n < network->s_layers[l]; n++)
        {
            if (DEBUG>=3)
                printf("%ld/%ld\n",n,network->s_layers[l]);
            output[l][n] =
                prop_neuron(network, &network->n_layers[l][n], output[l - 1]);
        }
        if (DEBUG>=2)
            print_inout(network->s_layers[l], output[l], "output[l]");
    }
    return output;  // correspond to all outputs layer by layer
}

/// @brief Apply back propagation on the param network
/// @param network The neural network pointer.
/// @param out output of each neuron layer by layer.
/// @param wanted_out The wanted output.
/// @return error delta which gives info about precision of the network.
float back_prop_network(Network* network, InOut* out, InOut wanted_out)
{
    float delta = 0.0;
    size_t nb_delta = 0;

    size_t size_oGrad = network->s_layers[2];
    InOut oGrad = layer_malloc(network, 2);
    assert(oGrad != NULL);

    // oGrad calc
    for (size_t n = 0; n < size_oGrad; n++) {
        oGrad[n] = (wanted_out[n] - out[2][n])
            /* * (out[2][n] - wanted_out[n]) */
            * sigmoid_prime(out[2][n]);
        delta += fabs(oGrad[n]);
        nb_delta++;
    }

    size_t size_hGrad = network->s_layers[1]; //hidden layer
    InOut hGrad = layer_malloc(network, 1); //hidden layer
    assert(hGrad != NULL);
    // hGrad calc
    for (size_t n = 0; n < size_hGrad; n++) {
        hGrad[n] = 0.0;
        for (size_t w = 0; w < size_oGrad; w++) {
            hGrad[n] +=
                network->w_layers[1][network->n_layers[1][n].w_index_next[w]]
                * oGrad[w];
        }
        hGrad[n] *= out[1][n] * (1-out[1][n]);
        delta += fabs(hGrad[n]);
        nb_delta++;
    }

    // weights + bias  calc
    //h -> o
    size_t size_ho_deltas = network->s_layers[1]*network->s_layers[2];
    Weights ho_deltas = (Weights)calloc(size_ho_deltas, sizeof(float));
    assert(ho_deltas != NULL);
    for (size_t n = 0; n < network->s_layers[1]; n++) {
        for (size_t w = 0; w < size_oGrad; w++) { 
            size_t index = network->n_layers[1][n].w_index_next[w];
            ho_deltas[index] = ETA * oGrad[w] * out[1][n];//----------

            network->w_layers[1][index]
                += ho_deltas[index];
            
            network->n_layers[2][w].bias += ho_deltas[index];
        }
    }
    //i -> h
    size_t size_ih_deltas = network->s_layers[0]*network->s_layers[1];
    Weights ih_deltas = (Weights)calloc(size_ih_deltas, sizeof(float));
    assert(ih_deltas != NULL);
    for (size_t n = 0; n < network->s_layers[0]; n++) {
        for (size_t w = 0; w < size_hGrad; w++)
        {
            size_t index = network->n_layers[0][n].w_index_next[w];
            ih_deltas[index] = ETA * hGrad[w] * out[0][n];

            network->w_layers[0][index]
                += ih_deltas[index];
            
            network->n_layers[1][w].bias += ih_deltas[index];
        }
    }

    float error_delta = (1./2.) * delta;
    if (DEBUG>=2) printf("error_delta = %f = %f * %f\n",
        error_delta,(1./2.),delta);
    print_free(oGrad,"oGrad");
    print_free(hGrad,"hGrad");
    print_free(ho_deltas,"ho_deltas");
    print_free(ih_deltas,"ih_deltas");
    return error_delta;
}

/* - - - - - - - - - - - - - - - - - - - - - - */

InOut layer_malloc(Network* network, size_t layer_num)
{
    return (InOut)malloc(sizeof(float) * network->s_layers[layer_num]);
}

/* - - - - - - - - - - - - - - - - - - - - - - */

void print_s_layers(size_t len, size_t* tab)
{
    printf("s: [%ld",tab[0]);
    for (size_t i = 1; i<len; i++) {
        printf(", %ld",tab[i]);
    }
    printf("]\n");
}

void print_weights(size_t len, Weights tab)
{
    printf("w: [%.3f",tab[0]);
    for (size_t i = 1; i<len; i++) {
        printf(", %.3f",tab[i]);
    }
    printf("]\n");
}

void print_inout(size_t len, InOut tab, char* s)
{
    printf("io %s:\t[%.3f", s, tab[0]);
    for (size_t i = 1; i<len; i++) {
        printf(", %.3f",tab[i]);
    }
    printf("]\n");
}

void print_neuron(Network* network, Neuron* neuron)
{
    printf("wl prev:%ld\n",neuron->wl_index_prev);
    printf("w prev:\t");
    if (neuron->w_index_prev != NULL) {
        for (size_t i = 0; i < network->s_layers[neuron->wl_index_prev]; i++) {
            printf("%ld\t", neuron->w_index_prev[i]);
        }
    }
    else
        printf("no previous layer");
    printf("\n");

    printf("bias:%f\tdelta%f\n",neuron->bias,neuron->delta);
    printf("wl next:%ld\n",neuron->wl_index_next);
    printf("w next:\t");
    if (neuron->w_index_next != NULL) {
        for (size_t i = 0; i < network->s_layers[neuron->wl_index_next]; i++) {
            printf("%ld\t", neuron->w_index_next[i]);
        }
    }
    else
        printf("no next layer");
    printf("\n");
}

void print_neurons(size_t len, Layer layer)
{
    printf("n: [%.3f",layer[0].bias);
    for (size_t i = 1; i<len; i++) {
        printf(", %.3f",layer[i].bias);
    }
    printf("]\n");
}

void print_network(Network* network)
{
    printf("\nnb_layers: %ld\n",network->nb_layers);
    print_s_layers(network->nb_layers,network->s_layers);
    for (size_t l = 0; l<network->nb_layers-1; l++) {
        print_neurons(network->s_layers[l],network->n_layers[l]);
        print_weights(
            network->s_layers[l]*network->s_layers[l+1],network->w_layers[l]);
    }
    print_neurons(
        network->s_layers[network->nb_layers-1],
        network->n_layers[network->nb_layers-1]);
    printf("\n");
}

void print_free(void* to_free, char* str)
{
    if (DEBUG>=3) printf("free(%s)\n", str);
    free(to_free);
}

/* - - - - - - - - - - - - - - - - - - - - - - */


//int main(int argc, char* argv[])
////int main()
//{
//    /* if (argc!=1+1)
//        errx(1,"Only one argument allowed"); */
//    
//    int iterations;
//    int load; //0 : no load, create network  |  1 : load network
//    if (argc < 2)
//        iterations = 10;
//    else
//        iterations = atoi(argv[1]);
//
//    if (argc < 3)
//        load = 0;
//    else
//        load = 1;
//
//    Network *network;
//    int nbin;
//    int nbhide;
//    int nbout;
//    if (!load)
//    {
//        srand(time(NULL));
//        nbin   = 28*28;
//        nbout  = 10;
//        nbhide = 
//            (nbin>nbout?1:-1)*(nbin - nbout)/2 
//            + (nbin>nbout?nbout:nbin);
//        size_t slayers[] = {nbin, nbhide, nbout};
//        //print_s_layers(3, slayers);
//        network = create_network(3, slayers);
//        init_network(network);
//    }
//    else {
//        network = load_network(argv[2]);
//    }
//    char *names[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
//
//    training(network, "save_numbers", names, 250, iterations,
//        "number_", ".png"); //               161
//
//    /* if (1+DEBUG>=2) print_network(network);
//
//    InOut inputs0 = (InOut)malloc(sizeof(float)*nbin);
//    for (int i = 0; i < nbin; i++)  // img : 
//    {                               //  1  1  1
//        inputs0[i] = i==4 ? 0 : 1;  //  1  0  1
//    }                               //  1  1  1
//
//    InOut inputs1 = (InOut)malloc(sizeof(float)*nbin);
//    for (int i = 0; i < nbin; i++)  // img : 
//    {                               //  0  1  0
//        inputs1[i] = i%3==1 ? 1 : 0;//  0  1  0
//    }                               //  0  1  0
//
//    InOut inputs4 = (InOut)malloc(sizeof(float)*nbin);
//    char str4[] = "101111001";
//    for (int i = 0; i < nbin; i++)  // img :
//    {                               //  1  0  1
//        inputs4[i] = str4[i]-'0';   //  1  1  1
//    }                               //  0  0  1
//    
//    InOut inputs7 = (InOut)malloc(sizeof(float)*nbin);
//    char str7[] = "111010100";
//    for (int i = 0; i < nbin; i++)  // img : 
//    {                               //  1  1  1
//        inputs7[i] = str7[i]-'0';   //  0  1  0
//    }                               //  1  0  0
//
//    InOut inputs_tab[] = {inputs0, inputs1, inputs4, inputs7}; */
///*     
//    char inputs_str_tab[nbout] = {
//        "1111110001100011000111111", // 0
//        "1111110001100011000111111", // 1 //pas encore fait
//        "1111110001100011000111111", // 2 //pas encore fait
//        "1111110001100011000111111", // 3 //pas encore fait
//        "1111110001100011000111111", // 4 //pas encore fait
//        "1111110001100011000111111", // 5 //pas encore fait
//        "1111110001100011000111111", // 6 //pas encore fait
//        "1111110001100011000111111", // 7 //pas encore fait
//        "1111110001100011000111111", // 8 //pas encore fait
//        "1111110001100011000111111", // 9 //pas encore fait
//    };
//    InOut inputs_tab[nbout];
//    for (int i = 0; i < nbout; i++) {
//        inputs_tab[i] = (InOut)malloc(sizeof(float)*nbin);
//        for (int pixel = 0; i < nbin; i++){
//            inputs_tab[i][pixel] = inputs_str_tab[i][pixel]-'0';
//        }
//    }
// */
//
//    free_network(network);
//
//    //end
//    return 0;
//}

/* // to test with gdb or valgrind (put it into Makefile)
    networkg:	Network/network.og
        gcc -g  Network/network.o  -lm -o network
    Network/network.og: Network/network.c Network/network.h
        gcc -g -Wall -Wextra -O3 -c -o Network/network.o Network/network.c
    */
