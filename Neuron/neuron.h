#ifndef Neuron_H
#define Neuron_H

#include <stdlib.h>
typedef struct Neuron
{
	//length of neuron
	//ex:
	// 
	//[1]  [n2]
	//			[fn]
	//[2]  [n1]
	//
	//lon={2,2,1}
	unsigned int l_len[10]; // 10 can be change
	size_t lent;
	//pointer of struct point first point begin
	//he look like [1],[2],[n2]<->[1],[1]<->[n1],[2]<->[n2],[2]<->[n1],
	//		[n2],[n1],[n2]<->[fn],[n1]<->[fn],
	//		[fn]
	double* val;
}Neuron;

// to calculate sigmoid
double sigmoid(double x);

// absolut value
double bas_val(double x);

//creat neuron 
Neuron* n_init(Neuron *n, unsigned int len[],size_t le);

// free neuron
void free_n(Neuron* n);

// send pointer for the first link for num neuron
double* first_link(Neuron* n,int num);

//send val of num neuron
double* val_neuron(Neuron* n,int num);

//print all neuron
void print_n(Neuron* n);

// send first of ancestor link
double* ancestor_link(Neuron* n, int num);

//save neuron
void save_Neuron(Neuron* n,char filename[]);

//load neuron
void load_Neuron(Neuron* n,char filename[]);

#endif
