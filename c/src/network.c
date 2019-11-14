#include "LMLP.h"
#include "error.h"
#include "utility.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void printNetwork(Network* net)
{
	printf("------------Weights------------\n");
	printf("wLayerNum[J][K]\n");
	for(int i = 1; i < net->layerCount; i++)
	{
		for(unsigned x = 0; x < net->w[i].j; x++)
		{
			for(unsigned y = 0; y < net->w[i].k; y++)
			{
				printf("w%d[%d][%d]: %4.3f\n", i, x, y, net->w[i].array[x][y]);
			}
		}
	}

	printf("\n------------Biases-------------\n");
	printf("bLayerNum[J]\n");
	for(int i = 1; i < net->layerCount; i++)
	{
		for(unsigned x = 0; x < net->b[i].j; x++)
		{
			for(unsigned y = 0; y < net->b[i].k; y++)
			{
				printf("b%d[%d]: %4.3f\n", i, x, net->b[i].array[x][y]);
			}
		}

	}

	printf("-------------------------------\n");
}

void printNetworkResult(Network* net, long label)
{
	printf("\nAeon %ld:\nfx(", label);
	for(unsigned int i = 0; i < net->layerSizeArray[0] - 1; i++)
	{
		printf("%4.3f, ", net->a[0].array[i][0]);
	}

	printf("%4.3f) = \n\t\t", net->a[0].array[net->layerSizeArray[0] - 1][0]);

	for(unsigned int i = 0; i < net->layerSizeArray[net->layerCount - 1] - 1; i++)
	{
		printf("%4.3f, ", net->a[net->layerSizeArray[net->layerCount - 1]].array[i][0]);
	}

	printf("%4.3f\n", net->a[net->layerSizeArray[net->layerCount - 1]].array[net->layerSizeArray[net->layerCount - 1] - 1][0]);

}



int initializeNetwork(unsigned int numberofLayers, Network* net, ...)
{
	seedGenerator();
	net->grad.unused = 1;
	net->avgGrad.unused = 1;

	net->layerCount = numberofLayers;
	net->layerSizeArray = calloc(numberofLayers, sizeof(unsigned int));

	net->z = calloc(numberofLayers, sizeof(Matrix*));
	net->y = calloc(net->layerSizeArray[net->layerCount - 1], sizeof(double));

	net->a = calloc(numberofLayers, sizeof(Matrix));
	net->grad.a = calloc(numberofLayers, sizeof(Matrix));
	net->avgGrad.a = calloc(numberofLayers, sizeof(Matrix));
	net->b = calloc(numberofLayers, sizeof(Matrix));
	net->grad.b = calloc(numberofLayers, sizeof(Matrix));
	net->avgGrad.b = calloc(numberofLayers, sizeof(Matrix));
	net->w = calloc(numberofLayers, sizeof(Matrix));
	net->grad.w = calloc(numberofLayers, sizeof(Matrix));
	net->avgGrad.w = calloc(numberofLayers, sizeof(Matrix));

	va_list args;
    va_start(args, net);
    int prev = -1;
    for(unsigned int i = 0; i < numberofLayers; i++)
    {
    	unsigned int len = va_arg(args, unsigned int);
    	net->layerSizeArray[i] = len;
    	intializeMatrix(&(net->z[i]), len, 1);
    	intializeMatrix(&(net->a[i]), len, 1);
    	intializeMatrix(&(net->b[i]), len, 1);

    	intializeMatrix(&(net->grad.a[i]), len, 1);
    	intializeMatrix(&(net->grad.b[i]), len, 1);

    	intializeMatrix(&(net->avgGrad.a[i]), len, 1);
    	intializeMatrix(&(net->avgGrad.b[i]), len, 1);

    	if(prev != -1)
    	{
    		intializeMatrix(&(net->w[i]), len, prev);
    		intializeMatrix(&(net->grad.w[i]), len, prev);
    		intializeMatrix(&(net->avgGrad.w[i]), len, prev);
    	}


    	randomizeMatrixValues(&(net->b[i]), getRand);
    	randomizeMatrixValues(&(net->w[i]), getRand);

    	prev = len;
    }



    //truths
    net->truthLen = 4;
    net->truthCount = 0;
  	net->truthOutput = calloc(net->truthLen, sizeof(double*));
    net->truthInput = calloc(net->truthLen, sizeof(double*));





    return 1;
}

void applyGradientToNetwork(Network* net, double granularity)
{
	granularity = fabs(granularity) * -1.0;

	for(unsigned int i = 1; i < net->layerCount; i++)
	{
		for(unsigned int it = 0; it < net->layerSizeArray[i]; it++)
		{
			net->b[i].array[it][0] += net->avgGrad.b[i].array[it][0] * granularity;
		}

		for(unsigned int m = 0; m < net->avgGrad.w[i].j; m++)
		{
			for(unsigned int n = 0; n < net->avgGrad.w[i].k; n++)
			{
				net->w[i].array[m][n] += net->avgGrad.w[i].array[m][n] * granularity;
			}
		}
	}

	net->avgGrad.unused = 1;
}

void insertTruth(Network* net, double* in, double* out, unsigned int inLen, size_t outLen)
{
	unsigned int 	inputLength = net->layerSizeArray[0],
					outputLength = net->layerSizeArray[net->layerCount - 1];
	if(inLen != inputLength || outLen != outputLength)
	{
		error();
	}
	net->truthInput[net->truthCount] = calloc(inputLength, sizeof(double));
	net->truthOutput[net->truthCount] = calloc(outputLength, sizeof(double));

	for(unsigned int i = 0; i < inputLength; i++)
	{
		net->truthInput[net->truthCount][i] = in[i];
	}

	for(unsigned int i = 0; i < outputLength;i++)
	{
		net->truthOutput[net->truthCount][i] = out[i];
	}

	net->truthCount++;

	if(net->truthCount == net->truthLen)
	{
		net->truthLen += 10;
		net->truthInput = realloc(net->truthInput, net->truthLen * sizeof(double*));
		net->truthOutput = realloc(net->truthOutput, net->truthLen * sizeof(double*));
	}




}

void setTruthState(Network* net)
{
	unsigned long t = random_0_to_n(net->truthCount);

	for(int i = 0; i < net->layerSizeArray[0]; i++)
	{
		net->a[0].array[i][0] = net->truthInput[t][i];
	}
	//copy for safety
	for(int i = 0; i < net->layerSizeArray[net->layerCount - 1]; i++)
	{
		net->y[i] = net->truthOutput[t][i];
	}
}




void backpropNetwork(Network* net)
{
	unsigned int layers = net->layerCount;
	unsigned int* layerSizes = net->layerSizeArray;
	GradientVector* grad = &(net->grad);
	//first lets find the cost w/ respect to the output layer's neurons activations
	// ∂C_∂a = 2(a_j - y_j)
	for(unsigned int it = 0; it < layerSizes[layers - 1]; it++)
	{
		grad->a[layers-1].array[it][0] = 2.0 * (net->a[layers-1].array[it][0] - net->y[it]);
	}

	for(unsigned int i = layers; i > 0; i--)
	{
		//now lets find the cost w/ respect to all of this layer's neurons' biases
		// ∂C_∂b = ∂C_∂a * ∂a_∂z * ∂z_∂b = 2(a - y) * dsigmoid(z) * 1
		for(unsigned int it = 0; it < layerSizes[i]; it++)
		{
			grad->b[i].array[it][0] = grad->a[i].array[it][0] * dsigmoid(net->z[i].array[it][0]);
		}

		//now find cost w/ repect to weights of this layer
		//∂C_∂w = ∂C_∂a * ∂a_∂z * ∂z_∂w = ∂C_∂b * ∂z_∂w = ∂C_∂b * a^(L-1)
		//∂C_∂b we calculated before, in the previous iterations
		for(unsigned int m = 0; m < grad->w[i].j; m++)
		{
			for(unsigned int n = 0; n < grad->w[i].k; n++)
			{
				grad->w[i].array[m][n] = grad->b[i].array[m][0] * (net->a[i - 1].array[n][0]);
			}
		}

		//we don't need to find the cost w/ respect to the inputs since we can't change the inputs
		if(i - 1 > 0)
		{
			//now find the derivative w/ respect to the previous layer's activations
			//∂C_∂aP = ∑j(∂C_∂b * ∂z_∂aP) = ∑j(∂C_∂b * (w_jk)^L)
			for(unsigned int k = 0; k < layerSizes[i - 1]; k++)
			{
				grad->a[i-1].array[k][0] = 0.0;
				for(unsigned int j = 0; j < layerSizes[i]; j++)
				{
					grad->a[i-1].array[k][0] += grad->b[i].array[j][0] * (net->w[i].array[j][k]);
				}
			}
		}

	}

	if(net->avgGrad.unused == 0) // if it is used
	{
		for(unsigned int i = layers - 1; i > 0; i--)
		{
			for(unsigned int it = 0; it < layerSizes[i]; it++)
			{
				net->avgGrad.b[i].array[it][0] += grad->b[i].array[it][0];
				net->avgGrad.b[i].array[it][0] /= 2.0;
			}

			for(unsigned int m = 0; m < grad->w[i].j; m++)
			{
				for(unsigned int n = 0; n < grad->w[i].k; n++)
				{
					net->avgGrad.w[i].array[m][n] += grad->w[i].array[m][n];
					net->avgGrad.w[i].array[m][n] /= 2.0;
				}
			}
		}
	}
	else
	{
		for(unsigned int i = layers - 1; i > 0; i--)
		{
			for(unsigned int it = 0; it < layerSizes[i]; it++)
			{
				net->avgGrad.b[i].array[it][0] = grad->b[i].array[it][0];
			}

			for(unsigned int m = 0; m < grad->w[i].j; m++)
			{
				for(unsigned int n = 0; n < grad->w[i].k; n++)
				{
					net->avgGrad.w[i].array[m][n] = grad->w[i].array[m][n];
				}
			}
		}
		net->avgGrad.unused = 0;
	}
}


void feedforwardNetwork(Network* net)
{
	for(int i = 1; i < net->layerCount; i++)
	{
		Matrix product = matrixMultiply(&(net->w[i]), &(net->a[i - 1]));
		matrixCopy(&(net->z[i]), &product);
		Matrix sum = matrixAdd(&(net->z[i]), &(net->b[i]));
		matrixCopy(&(net->z[i]), &sum);
		matrixCopy(&(net->a[i]), &(net->z[i]));
		applyFunctionToMatrix(&(net->a[i]), sigmoid);
	}
}






void runNetwork(Network* net, long cycles, double granularity, int samplesPerGradient, int printMod)
{
	cycles = cycles/samplesPerGradient + cycles%samplesPerGradient;
	for(long i = 0; i < cycles; i++)
	{
		for(int j = 0; j < samplesPerGradient; j++)
		{
			setTruthState(net);
			feedforwardNetwork(net);
			if(printMod != -1 && ((i*samplesPerGradient)+j)%printMod == 0)
				printNetworkResult(net, (i*samplesPerGradient)+j);
			backpropNetwork(net);
		}
		applyGradientToNetwork(net, granularity);
	}
}










