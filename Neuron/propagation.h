#ifndef PROPAGATION_H
#define PROPAGATION_H

int num_first_neuron_layer(Neuron* n, size_t layer);

void propagation(Neuron* n);

double retropropagation(Neuron* n, double wanted_out);

void clean(Neuron* n);

void back_and_forth(
	Neuron* n, double inputs[], int nb_inputs, double wanted_out);

void training_XOR(Neuron* n);

void init_link(Neuron* n);

#endif
