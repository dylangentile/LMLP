#ifndef LMLP_H
#define LMLP_H
#include "matrix.h"
#include <stddef.h>


typedef struct
{
	int unused;
	Matrix *b, *w, *a;
} GradientVector;

typedef struct 
{
	double** truthInput;
	double** truthOutput;
	unsigned int truthCount;
	size_t truthLen;
	unsigned int  layerCount;
	unsigned int* layerSizeArray;
	GradientVector grad, avgGrad;
	Matrix *z, *a, *b, *w;
	double* y;
} Network;

//returns non 0 on success
int initializeNetwork(unsigned int numberofLayers, Network* net, ...);
void applyGradientToNetwork(Network* net, double granularity);
void feedforwardNetwork(Network* net);
void backpropNetwork(Network* net);
void printNetwork(Network* net);
void printNetworkResult(Network* net, long label);
void insertTruth(Network* net, double* in, double* out, unsigned int inLen, size_t outLen);

void setTruthState(Network* net);

void runNetwork(Network* net, long cycles, double granularity, int samplesPerGradient, int printMod);

#endif //LMLP_H