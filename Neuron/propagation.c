#include <time.h>
#include <stdio.h>
#include <math.h>
#include "neuron.h"
#include "propagation.h"

const double ALPHA = 0.1; //learning rate


int num_first_neuron_layer(Neuron* n, size_t layer)
{
	int num = 0;
	for (size_t i = 0; i < layer; i++) {
		num += (n->l_len)[i];
	}
	return num+1;
}

void propagation(Neuron* n) 
{
	double* n1; //first neuron of one layer
	double* n2 = n->val; //first neuron of next layer
	double* link; //link incremented to calculate neurons
	int num_n2;
	int nb_layer;
	int nb_next_layer;

	size_t layer;
	for (layer = 0; layer < n->lent; layer++) {
		n1 = n2;
		num_n2 = num_first_neuron_layer(n, layer+1);
		n2 = val_neuron(n, num_n2);
		link = ancestor_link(n, num_n2); // [n1] <-> [n2]
		nb_layer = (n->l_len)[layer];
		nb_next_layer = (n->l_len)[layer+1];
		for (int i = 0; i<nb_layer; i++) {
			for (int j = 0; j<nb_next_layer; j++) {
				*(n2+j) += *(n1+i) * *(link+i*nb_next_layer+j);//for each link
			}
		}
		for (int j = 0; j<nb_next_layer; j++) {
			*(n2+j) = sigmoid(*(n2+j)); //apply sigmoid for each sum
		}
	}
}

double retropropagation(Neuron* n, double wanted_out)
{
	int num_n = num_first_neuron_layer(n, n->lent -1); //num of last neuron
	double* last_n = val_neuron(n, num_n); //pointer to last neuron

	int nb_neurons = 0;
	for (size_t i = 0; i<n->lent; i++) {
		nb_neurons += (n->l_len)[i];
	}

	double* deltas = (double*)calloc(nb_neurons,(size_t)sizeof(double));
	deltas[nb_neurons-1] = wanted_out - *last_n;
	
	for (int layer = n->lent-2; layer>0; layer--) {
		int nb_n_l = (n->l_len)[layer]; //nb of neurons in layer
		num_n -= nb_n_l;
		for (int neuron = 0; neuron<nb_n_l; neuron++) {
			double val_n = *val_neuron(n, num_n+neuron);
			double* link = first_link(n, num_n+neuron);
			for (unsigned int i = 0; i<(n->l_len)[layer+1]; i++) {
				deltas[num_n+neuron-1] 
				+= val_n * (wanted_out-val_n) 
					* *(link+i) * deltas[num_n+nb_n_l-1+i];
			}
		}
	}


	num_n = 1;
	for (unsigned int layer = 0; layer<n->lent-1; layer++) {
		int nb_n_l = (n->l_len)[layer]; //nb of neurons in layer
		for (int neuron = 0; neuron<nb_n_l; neuron++) {
			double val_n = *val_neuron(n, num_n+neuron);
			double* link = first_link(n, num_n+neuron);
			for (unsigned int i = 0; i<(n->l_len)[layer+1]; i++) {
				*(link+i) += ALPHA*val_n*deltas[num_n+nb_n_l-1+i];
			}
		}
		num_n += nb_n_l;
	}
	double res_delta = deltas[nb_neurons-1];
	//printf("==================free=================\n");
	free(deltas);
	return res_delta;
}

void clean(Neuron* n)
{
	int nb_neurons = 0;
	for (size_t i = 0; i<n->lent; i++) {
		nb_neurons += (n->l_len)[i];
	}
	for (int neuron = 1; neuron<=nb_neurons; neuron++) {
		*val_neuron(n, neuron) = 0.0;
	}
}

void back_and_forth(
	Neuron* n, double inputs[], int nb_inputs, double wanted_out)
{
	for (int i = 0; i<nb_inputs; i++) {
		*val_neuron(n, i) = inputs[i];
	}

	propagation(n);

	//print_n(n);
	double res_delta = retropropagation(n, wanted_out);
	//print_n(n);

	//printf("i1:%.1lf\ti2:%.1lf\two:%.1lf\t",inputs[0],inputs[1],wanted_out);
	//printf("delta : %s%lf\t", res_delta<0 ? "" :" ", res_delta);
	double abs_res_delta = (res_delta <0) ? -res_delta : res_delta;
	for (int i = 0; i<(int)(abs_res_delta*100); i++)
		//printf(i==50 ? "|" : "-");
	//printf("\n");

	clean(n);
}

void training_XOR(Neuron* n) 
{
	int input1;
	int input2;
	int nb_inputs = 2;
	srand(time(NULL));
	for (int i = 0; i<1000000; i++){
		input1 = rand()%2;
		input2 = rand()%2;
		/*if (input1 && input2)
			print_n(n);*/
		/*double wanted_out = 
			((input1 || input2) == (input1 && input2)) ? (0.0) : (1.0);
		double inputs[] = {(double)(input1 || input2), 
							(double)((input1 && input2))};*/
		double wanted_out = (input1 == input2) ? (0.0) : (1.0);
		double inputs[] = {(double)(input1), (double)(input2)};
		
		back_and_forth(
			n, inputs, nb_inputs, wanted_out);
		/*if (input1 && input2) {
			print_n(n);
			char c = ' ';
			int trash = scanf("%c",&c);
			printf("%d",trash);
		}*/
			
	}
}

void init_link(Neuron* n)
{
	srand(time(NULL));
	double* link;
	double lower, upper, number, scaled;
	int p, x;
	for ( int layer=0; layer < (int)n->lent-1; layer++ ) {
		// number of nodes in the previous layer
		p = n->l_len[layer];
		// number of nodes in the next layer
		x = n->l_len[layer+1];
		// calculate the range for the weights
		upper = sqrt(6.0) / sqrt(p + x);
		lower = -upper;
		link = first_link(n,num_first_neuron_layer(n,layer));
		for ( int i=0; i<p*x; i++) {
			// generate random number uniformly in [0,1]
			number = (double)rand() / (double)RAND_MAX;
			// scale to the desired range
			scaled = lower + number * (upper - lower);
			// initialize weight
			*(link+i) = scaled;
		}
	}
}

int main()
{
	Neuron N1;
	unsigned int list[]={2,2,1};
	n_init(&N1,list,(size_t)3);

	/*double* link;
	link = first_link(&N1,1);
	*link = 0.5;
	*(link+1) = -1.0;
	*(link+2) = 1.5;
	*(link+3) = -2.0;

	link = first_link(&N1,3);
	*link = 1.0;
	*(link+1) = -1.0;
	*(link+2) = 3.0;
	*(link+3) = -4.0;

	link = first_link(&N1,5);
	*link = 1.0;
	*(link+1) = -3.0;*/

	/*print_n(&N1);

	back_and_forth(&N1);

	print_n(&N1);*/

	//print_n(&N1);

	init_link(&N1);

	training_XOR(&N1);

	save_Neuron(&N1,"xor");
	
	//free_n(&N1);
	//print_n(&N1);
	//printf("fin du main\n");
	return 0;
}
