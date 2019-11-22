#include "network.h"
#include "function.h"
#include <cstdio>
#include <cmath> //abs()
#include <map>
#include <iterator>
using namespace std;

Network::Network(const std::vector<unsigned int>& layerSizes) : y(0)
{

	seedGenerator();
	layerSizeArray = layerSizes;
	numberOfLayers = layerSizes.size();
	weightsPerLayer = new unsigned int[numberOfLayers];
	a = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		a[i] = new Matrix(layerSizes[i], 1);
		//a[i]->print();
	}

	z = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		z[i] = new Matrix(layerSizes[i], 1);
	}
	
	b = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		b[i] = new Matrix(layerSizes[i], 1, getRand);
	}


	w = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		if(i == 0)
		{
			w[i] = new Matrix(1, 1);
			weightsPerLayer[0] = 1;
		}
		else
		{
			w[i] = new Matrix(layerSizes[i], layerSizes[i - 1], getRand);
			weightsPerLayer[i] = layerSizes[i] * layerSizes[i - 1];
		}
	}

	grad.b = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		grad.b[i] = new Matrix(layerSizes[i], 1);
	}

	grad.a = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		grad.a[i] = new Matrix(layerSizes[i], 1);
	}

	grad.w = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		if(i == 0)
		{
			grad.w[i] = new Matrix(1, 1);
		}
		else
		{
			grad.w[i] = new Matrix(layerSizes[i], layerSizes[i - 1]);
		}
	}


	theGradient.b = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		theGradient.b[i] = new Matrix(layerSizes[i], 1);
	}

	theGradient.a = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		theGradient.a[i] = new Matrix(layerSizes[i], 1);
	}

	theGradient.w = new Matrix*[numberOfLayers];
	for(unsigned i = 0; i < numberOfLayers; i++)
	{
		if(i == 0)
		{
			theGradient.w[i] = new Matrix(1, 1);
		}
		else
		{
			theGradient.w[i] = new Matrix(layerSizes[i], layerSizes[i - 1]);
		}
	}

	y = new double[layerSizeArray[numberOfLayers-1]];




}

Network::~Network()
{
	//todo: do this...
}

void
Network::print()
{
	printf("------------Weights------------\n");
	printf("wLayerNum[J][K]\n");
	for(int i = 1; i < numberOfLayers; i++)
	{
		unsigned m = 0, n = 0;
		w[i]->size(m, n);
		for(unsigned x = 0; x < m; x++)
		{
			for(unsigned y = 0; y < n; y++)
			{
				printf("w%d[%d][%d]: %4.3f\n", i, x, y, w[i]->get(x,y));
			}
		}
	}

	printf("\n------------Biases-------------\n");
	printf("bLayerNum[J]\n");
	for(int i = 1; i < numberOfLayers; i++)
	{
		unsigned m = 0, n = 0;
		b[i]->size(m, n);
		for(unsigned x = 0; x < m; x++)
		{
			for(unsigned y = 0; y < n; y++)
			{
				printf("b%d[%d]: %4.3f\n", i, x, b[i]->get(x,y));
			}
		}

	}

	printf("-------------------------------\n");
						

}

//glimpse the nasty looking code... beware!
void 
Network::printResult(long label)
{
	printf("\nAeon %ld:\nfx(", label);
	for(int i = 0; i < layerSizeArray[0] - 1; i++)
	{
		printf("%4.3f, ", (*a[0])[i][0]);
	}

	printf("%4.3f) = \n\t\t", (*a[0])[layerSizeArray[0] - 1][0]);

	for(int i = 0; i < layerSizeArray[numberOfLayers - 1] - 1; i++)
	{
		printf("%4.3f, ", (*a[layerSizeArray[numberOfLayers - 1]])[i][0]);
	}

	printf("%4.3f\n", (*a[layerSizeArray[numberOfLayers - 1]])[layerSizeArray[numberOfLayers - 1] - 1][0]);
}

bool 
Network::passTruth(std::vector<double> input, std::vector<double> real)
{
	/*if(sizeof(input)/sizeof(input[0]) != layerSizeArray[0])
		return false;
	
	if(sizeof(realVal)/sizeof(realVal) != layerSizeArray[numberOfLayers-1])
		return false;*/

	//theTruth = trueth;
	//theTruth.insert(make_pair(input, realVal));


	if(input.size() != layerSizeArray[0])
		return false;

	if(real.size() != layerSizeArray[numberOfLayers-1])
		return false;

	double* in = new double[input.size()];
	double* rl = new double[real.size()];

	for(unsigned i = 0; i < input.size(); i++)
	{
		in[i] = input[i];
	}
	for(unsigned i = 0; i < real.size(); i++)
	{
		rl[i] = real[i];
	}


	theTruth.insert(make_pair(in, rl));
	return true;
}


void
Network::getInput()
{
	unordered_map<double*, double*>::iterator item = theTruth.begin();
	advance(item, random_0_to_n(theTruth.size()));
	//len = sizeof(item->first)/sizeof(item->first[0]);
	y = item->second;
	for(unsigned i = 0; i < layerSizeArray[0]; i++)
	{
		a[0]->set(i, 0, item->first[i]);
	}
}




void
Network::feedforward(/*const std::vector<double>& input, double* real*/)
{
	/*
	if(input.size() != layerSizeArray[0])
	{
		throw "your input size doesn't match your neural network's input size";
	}
	for(unsigned i = 0; i < layerSizeArray[0]; i++)
	{
		a[0]->set(i, 0, input[i]);
	}

	if(real != nullptr)
	{
		if((sizeof(real) / sizeof(real[0])) != layerSizeArray[0])
		{
			throw "the true array is the wrong length!";
		}
		y = real;

		throw "importing custom reals into feedforward is currently unsupported!";
		//todo: fix
	}


	a[0]->print();
	*/


	for(int i = 1; i < numberOfLayers; i++)
	{
		*z[i] = (*w[i]) * (*a[i - 1]);
		*z[i] = *z[i] + *b[i];
		*a[i] = z[i]->applyFunctionToAll(sigmoid);
	}

	//a[2]->print();
	

}


void 
Network::applyGradient(double granularity)
{
	if(granularity == 0)
	{
		granularity = theGradient.mMod;
	}

	granularity = fabs(granularity) * -1.0;
	
	for(int i = 1; i < numberOfLayers; i++)
	{
		for(int it = 0; it < layerSizeArray[i]; it++)
		{
			(*b[i])[it][0] +=  (*theGradient.b[i])[it][0] * granularity;
		}

		for(int m = 0; m < w[i]->w(); m++)
		{
			for(int n = 0; n < w[i]->z(); n++)
			{
				(*w[i])[m][n] += (*theGradient.w[i])[m][n] * granularity;
			}
		}
	}

	theGradient.unused = true;
}





void 
Network::backprop()
{

	//first lets find the cost w/ respect to the output layer's neurons activations
	// ∂C_∂a = 2(a_j - y_j)
	for(int it = 0; it < layerSizeArray[numberOfLayers-1]; it++)
	{
		(*grad.a[numberOfLayers-1])[it][0] = 2.0 * ((*a[numberOfLayers-1])[it][0] - y[it]);
	}

	for(int i = numberOfLayers - 1; i >= 1; i--)
	{
		//now lets find the cost w/ respect to all of this layer's neurons' biases
		// ∂C_∂b = ∂C_∂a * ∂a_∂z * ∂z_∂b = 2(a - y) * dsigmoid(z) * 1
		for(int it = 0; it < layerSizeArray[i]; it++)
		{
			(*grad.b[i])[it][0] = (*grad.a[i])[it][0] * dsigmoid((*z[i])[it][0]);
		}

		//now find cost w/ repect to weights of this layer
		//∂C_∂w = ∂C_∂a * ∂a_∂z * ∂z_∂w = ∂C_∂b * ∂z_∂w = ∂C_∂b * a^(L-1)
		//∂C_∂b we calculated before, in the previous iteration

		for(int m = 0; m < grad.w[i]->w(); m++)
		{
			for(int n = 0; n < grad.w[i]->z(); n++)
			{
				(*grad.w[i])[m][n] = ((*grad.b[i])[m][0]) * ((*a[i - 1])[n][0]);
				//(*grad.w[i])[m][n] = 2.0;
			}
		}

		//we don't need to find the cost w/ respect to the inputs
		if(i-1 >= 1)
		{
			//now find the derivative w/ respect to the previous layer's activations
			//∂C_∂aP = ∑j(∂C_∂b * ∂z_∂aP) = ∑j(∂C_∂b * (w_jk)^L)
			for(int k = 0; k < layerSizeArray[i - 1]; k++)
			{
				(*grad.a[i-1])[k][0] = 0.0;
				for(int j = 0; j < layerSizeArray[i]; j++)
				{
					(*grad.a[i-1])[k][0] = (*grad.b[i])[j][0] * (*w[i])[j][k];
				}
			}
		}
	}

	if(!theGradient.unused)
	{
		for(int i = numberOfLayers - 1; i >= 1; i--)
		{
			
			for(int it = 0; it < layerSizeArray[i]; it++)
			{
				(*theGradient.b[i])[it][0] += (*grad.b[i])[it][0];
				(*theGradient.b[i])[it][0] /= 2.0;
			}

			for(int m = 0; m < grad.w[i]->w(); m++)
			{
				for(int n = 0; n < grad.w[i]->z(); n++)
				{
					(*theGradient.w[i])[m][n] += (*grad.w[i])[m][n];
					(*theGradient.w[i])[m][n] /= 2.0;
				}
			}
		}
	}
	else
	{
		for(int i = numberOfLayers - 1; i >= 1; i--)
		{
			
			for(int it = 0; it < layerSizeArray[i]; it++)
			{
				(*theGradient.b[i])[it][0] = (*grad.b[i])[it][0];
			}

			for(int m = 0; m < grad.w[i]->w(); m++)
			{
				for(int n = 0; n < grad.w[i]->z(); n++)
				{
					(*theGradient.w[i])[m][n] = (*grad.w[i])[m][n];
				}
			}
		}
		theGradient.unused = false;
	}
}

void
Network::run(long cycles, double granularity, int samplesPerGradient, int printMod)
{
	cycles = cycles/samplesPerGradient + cycles%samplesPerGradient;
	for(long i = 0; i < cycles; i++)
	{
		for(int j = 0; j < samplesPerGradient; j++)
		{
			getInput();
			feedforward();
			if(printMod != -1 && ((i*samplesPerGradient)+j)%printMod == 0)
				printResult((i*samplesPerGradient)+j);
			backprop();
		}
		applyGradient(granularity);
	}

}
























