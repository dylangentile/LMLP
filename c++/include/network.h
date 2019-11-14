#pragma once
#include "matrix.h"
#include <vector>
#include <unordered_map>

typedef struct
{
	bool unused = true;
	Matrix **b, **w, **a;
	double mMod = 1;

} GradientVector;


class Network
{
public:
	Network(const std::vector<unsigned int>& layerSizes);
	~Network();
	void feedforward(/*const std::vector<double>& input, double* real = nullptr*/);
	void backprop();
	void run(long cycles, double granularity, int samplesPerGradient, int printMod = 4);
	void applyGradient(double granularity = 0);
	void print();
	bool passTruth(std::vector<double> input, std::vector<double> real);


	void getInput();

	void printResult(long label = -1);




private:
	std::vector<unsigned int> layerSizeArray;
	unsigned numberOfLayers;
	unsigned *weightsPerLayer; //weight layer correspond w/ neurons they output to
	double* y;
	Matrix** z, **a, **b; // layer X n-th neuron
	Matrix** w;		// array of weightMatricies layer X j X k
	std::unordered_map<double*, double*> theTruth;
	GradientVector grad, theGradient;
};