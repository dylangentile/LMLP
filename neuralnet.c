#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


double getRand()
{
	return (double)rand()/RAND_MAX*2.0-1.0;

}

typedef struct
{
    double weights1[2][2], weights2[1][2];
    double bias1[2], bias2[1];
    double gradientMod;
} GradientVector;


typedef struct 
{
	double y;
	double a0[2], a1[2], a2[1];
	double z0[2], z1[2], z2[1];
	double weights1[2][2], weights2[1][2];
	double bias1[2], bias2[1];

	GradientVector mGradient;

} Network;


double sigmoid(double x)
{
	return 1.0/(1.0+exp(-1.0*x));
}

double dsigmoid(double x)
{
	double f = sigmoid(x);
	return f*(1-f);
}




void initializeNetwork(Network* theNet, double gradientModifier, unsigned seed)
{
	//srand(5);
	srand(seed);
	theNet->weights1[0][0] = getRand();
	theNet->weights1[0][1] = getRand();

	theNet->weights1[1][0] = getRand();
	theNet->weights1[1][1] = getRand();

	theNet->weights2[0][0] = getRand();
	theNet->weights2[0][1] = getRand();

	theNet->bias1[0] = getRand();
	theNet->bias1[1] = getRand();
	
	theNet->bias2[0] = getRand();

	theNet->mGradient.weights1[0][0] = 0.0;
	theNet->mGradient.weights1[0][1] = 0.0;
	theNet->mGradient.weights1[1][0] = 0.0;
	theNet->mGradient.weights1[1][1] = 0.0;

	theNet->mGradient.weights2[0][0] = 0.0;
	theNet->mGradient.weights2[0][1] = 0.0;

	theNet->mGradient.bias1[0] = 0.0;
	theNet->mGradient.bias1[1] = 0.0;

	theNet->mGradient.bias2[0] = 0.0;

	theNet->mGradient.gradientMod = gradientModifier;






}

void getInput(Network* theNet)
{
	int random = rand()%4;
	if(random == 0)
	{
		theNet->a0[0] = 0.0;
		theNet->a0[1] = 0.0;
		theNet->y = 0.0;
	}

	
	if(random == 1)
	{
		theNet->a0[0] = 1.0;
		theNet->a0[1] = 0.0;
		theNet->y = 1.0;
	}

	if(random == 2)
	{

		theNet->a0[0] = 0.0;
		theNet->a0[1] = 1.0;
		theNet->y = 1.0;
	}

	if(random == 3)
	{
		theNet->a0[0] = 1.0;
		theNet->a0[1] = 1.0;
		theNet->y = 0.0;
	}
}


void feedforward(Network* theNet)
{

	//find layer 1 activations
	theNet->z1[0] = theNet->a0[0] * theNet->weights1[0][0] + theNet->a0[1] * theNet->weights1[0][1] + theNet->bias1[0];
	theNet->a1[0] = sigmoid(theNet->z1[0]);

	theNet->z1[1] = theNet->a0[0] * theNet->weights1[1][0] + theNet->a0[1] * theNet->weights1[1][1] + theNet->bias1[1];
	theNet->a1[1] = sigmoid(theNet->z1[1]);
	//find layer 2 activations (output)
	theNet->z2[0] = theNet->a1[0] * theNet->weights2[0][0] + theNet->a1[1] * theNet->weights2[0][1] + theNet->bias2[0];
	theNet->a2[0] = sigmoid(theNet->z2[0]);




}




void backprop(Network* theNet)
{
	//cost with respect to weight2
	double c_w2_00 = (theNet->a1[0]) * dsigmoid(theNet->z2[0]) * 2.0 * (theNet->a2[0] - theNet->y);
	double c_w2_01 = (theNet->a1[1]) * dsigmoid(theNet->z2[0]) * 2.0 * (theNet->a2[0] - theNet->y);

	//cost with respect to b2
	double c_b2_0 = (dsigmoid(theNet->z2[0]))  * 2.0 *  (theNet->a2[0] - theNet->y);

	//cost with respect to activations of layer 1
	double c_a1_0 = theNet->weights2[0][0] * (dsigmoid(theNet->z1[0]))  * 2.0 *  (theNet->a2[0] - theNet->y);
	double c_a1_1 = theNet->weights2[0][1] * (dsigmoid(theNet->z1[1]))  * 2.0 *  (theNet->a2[0] - theNet->y);


	double c_b1_0 = c_a1_0 * dsigmoid(theNet->z1[0]);
	double c_b1_1 = c_a1_1 * dsigmoid(theNet->z1[1]);

	double c_w1_00 = c_a1_0 * dsigmoid(theNet->z1[0]) * theNet->a0[0];
	double c_w1_01 = c_a1_0 * dsigmoid(theNet->z1[0]) * theNet->a0[1];

	double c_w1_10 = c_a1_1 * dsigmoid(theNet->z1[1]) * theNet->a0[0];
	double c_w1_11 = c_a1_1 * dsigmoid(theNet->z1[1]) * theNet->a0[1];


	theNet->mGradient.weights2[0][0] += c_w2_00;
	theNet->mGradient.weights2[0][0] = theNet->mGradient.weights2[0][0]/2.0;

	theNet->mGradient.weights2[0][1] += c_w2_01;
	theNet->mGradient.weights2[0][1] = theNet->mGradient.weights2[0][1]/2.0;

	theNet->mGradient.bias2[0] += c_b2_0;
	theNet->mGradient.bias2[0] = theNet->mGradient.bias2[0]/2.0;

	theNet->mGradient.weights1[0][0] += c_w1_00;
	theNet->mGradient.weights1[0][0] = theNet->mGradient.weights1[0][0]/2.0;

	theNet->mGradient.weights1[0][1] += c_w1_01;
	theNet->mGradient.weights1[0][1] = theNet->mGradient.weights1[0][1]/2.0;

	theNet->mGradient.weights1[1][0] += c_w1_10;
	theNet->mGradient.weights1[1][0] = theNet->mGradient.weights1[1][0]/2.0;

	theNet->mGradient.weights1[1][1] += c_w1_11;
	theNet->mGradient.weights1[1][1] = theNet->mGradient.weights1[1][1]/2.0;

	theNet->mGradient.bias1[0] += c_b1_0;
	theNet->mGradient.bias1[0] /= 2.0;

	theNet->mGradient.bias1[1] += c_b1_1;
	theNet->mGradient.bias1[1] /= 2.0;
}


void adjustWeights(Network* theNet)
{
	theNet->bias1[0] -= theNet->mGradient.bias1[0] * theNet->mGradient.gradientMod;
	theNet->bias1[1] -= theNet->mGradient.bias1[1] * theNet->mGradient.gradientMod;

	theNet->bias2[0] -= theNet->mGradient.bias2[0] * theNet->mGradient.gradientMod;

	theNet->weights1[0][0] -= theNet->mGradient.weights1[0][0] * theNet->mGradient.gradientMod;
	theNet->weights1[0][1] -= theNet->mGradient.weights1[0][1] * theNet->mGradient.gradientMod;
	theNet->weights1[1][0] -= theNet->mGradient.weights1[1][0] * theNet->mGradient.gradientMod;
	theNet->weights1[1][1] -= theNet->mGradient.weights1[1][1] * theNet->mGradient.gradientMod;

	theNet->weights2[0][0] -= theNet->mGradient.weights2[0][0] * theNet->mGradient.gradientMod;
	theNet->weights2[0][1] -= theNet->mGradient.weights2[0][1] * theNet->mGradient.gradientMod;


}

void printNetwork(Network* theNet)
{
	printf("------------------------\n");
	printf("Biases:\n");
	printf("in\t%4.3f\n", theNet->bias1[0]);
	printf("\t\t%4.3f\n", theNet->bias2[0]);
	printf("in\t%4.3f\n", theNet->bias1[1]);

	printf("------------------------\n");
	printf("Weights:\n");
	printf("weights1[0][0]: %4.3f\n", theNet->weights1[0][0]);
	printf("weights1[0][1]: %4.3f\n", theNet->weights1[0][1]);
	printf("weights1[1][0]: %4.3f\n", theNet->weights1[1][0]);
	printf("weights1[1][1]: %4.3f\n", theNet->weights1[1][1]);
	printf("weights2[0][0]: %4.3f\n", theNet->weights2[0][0]);
	printf("weights2[0][1]: %4.3f\n", theNet->weights2[0][1]);
	printf("------------------------\n");
}


void printResults(Network* theNet, int aeon)
{
	printf("\nAeon %d:\nf(%4.3f, %4.3f) = %4.3f\n", aeon, theNet->a0[0], theNet->a0[1], theNet->a2[0]);
}

void printFinal(Network* theNet)
{
	theNet->a0[0] = 0.0;
	theNet->a0[1] = 0.0;
	theNet->y = 0.0;
	feedforward(theNet);
	printf("FINAL RESULTS:\nf(%4.3f, %4.3f) = %4.3f\n",  theNet->a0[0], theNet->a0[1], theNet->a2[0]);

	theNet->a0[0] = 1.0;
	theNet->a0[1] = 0.0;
	theNet->y = 1.0;
	feedforward(theNet);
	printf("f(%4.3f, %4.3f) = %4.3f\n", theNet->a0[0], theNet->a0[1], theNet->a2[0]);

	theNet->a0[0] = 0.0;
	theNet->a0[1] = 1.0;
	theNet->y = 1.0;
	feedforward(theNet);
	printf("f(%4.3f, %4.3f) = %4.3f\n", theNet->a0[0], theNet->a0[1], theNet->a2[0]);

	theNet->a0[0] = 1.0;
	theNet->a0[1] = 1.0;
	theNet->y = 0.0;
	feedforward(theNet);
	printf("f(%4.3f, %4.3f) = %4.3f\n", theNet->a0[0], theNet->a0[1], theNet->a2[0]);



}


void exportNetwork()
{
	
}

int main(int argc, char const *argv[])
{
	if(argc < 3)
	{
		printf("Usage: %s iterations/4 gradientModifier rand-seed(optional)\n", argv[0]);
		return -1;
	}


	int totalAeons = atoi(argv[1]);
	totalAeons = (totalAeons/4) + (totalAeons%4);

	double gradientModifier = atof(argv[2]);
	unsigned randSeed = time(NULL);
	if(argc == 4)
	{
		randSeed = (unsigned)atoi(argv[3]);
	}


	printf("%d random seed\n", randSeed);

	Network myNet;
	initializeNetwork(&myNet, gradientModifier, randSeed);

	printNetwork(&myNet);

	for(int i = 0; i < totalAeons; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			getInput(&myNet);
			feedforward(&myNet);
			if(((i*4)+j)%50 == 0)
				printResults(&myNet,(i*4)+j);
			backprop(&myNet);
		}
		adjustWeights(&myNet);
	}


	printNetwork(&myNet);
	printf("\n\n");
	printFinal(&myNet);
	


	



	return 0;
}



