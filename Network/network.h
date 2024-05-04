#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>

// learning rate
#define ETA 0.50

//const size_t MAX_LAYERS = 10;
#define MAX_LAYERS 10

//numbers 0 to 9 included
#define MAX_NUM 10

// debug level
#define DEBUG 0 //0<-->3


/// @brief 
typedef struct
{
    size_t wl_index_prev;
    size_t* w_index_prev;

    float bias;
    float delta;

    size_t wl_index_next;
    size_t* w_index_next;
}Neuron;


/// @brief Layer is a Neuron's tab
typedef Neuron* Layer;


/// @brief Weights is a float's tab
typedef float* Weights;


/// @brief InOut is a float's tab
typedef float* InOut;


/// @brief The network
typedef struct
{
    size_t nb_layers;
    size_t s_layers[MAX_LAYERS];
    Layer n_layers[MAX_LAYERS];
    Weights w_layers[MAX_LAYERS-1];
}Network;



// -----------------------------------

Neuron create_neuron(Network* network, size_t l_index, size_t n_index);

Layer create_neuron_layer(Network* network, size_t index);

Network* create_network(size_t nb_layers, size_t layers_sizes[]);


void free_network(Network* network);


void init_weights(Network* network);

void init_bias(Network *network);

Network* init_network(Network* network);


Network* load_network(char filename[]);

void save_network(Network *network, char filename[]);


// to calculate sigmoid
float sigmoid(float x);
// to calculate prime sigmoid
float sigmoid_prime(float x);

//random sign
int randsign();

/* - - - - - - - - - - - - - - - - - - - - - - */

//region prop/backprop

/// @brief Calculate the prop activation of the neuron given.
/// @param network The neural network pointer.
/// @param neuron The neuron which output is calculated.
/// @param inputs The inputs of the neuron.
/// @return The output value of the neuron.
float prop_neuron(Network *network, Neuron *neuron, InOut inputs);

/// @brief Apply the prop on all neurons exept in input layer.
/// @param network The neural network pointer.
/// @return output of each neuron layer by layer.
InOut* prop_network(Network *network, InOut inputs);

/// @brief Apply back propagation on the param network
/// @param network The neural network pointer.
/// @param out output of each neuron layer by layer.
/// @param wanted_out The wanted output.
/// @return error delta which gives info about precision of the network.
float back_prop_network(Network *network, InOut* out, InOut wanted_out);

//endregion prop/backprop

/* - - - - - - - - - - - - - - - - - - - - - - */

InOut layer_malloc(Network *network, size_t layer_num);

/* - - - - - - - - - - - - - - - - - - - - - - */

//region print

void print_s_layers(size_t len, size_t* tab);

void print_weights(size_t len, Weights tab);

void print_inout(size_t len, InOut tab, char* s);

void print_neuron(Network* network, Neuron* neuron);

void print_neurons(size_t len, Layer layer);

void print_network(Network* network);

void print_free(void *to_free, char *str);

//endregion print

/* - - - - - - - - - - - - - - - - - - - - - - */

#endif
