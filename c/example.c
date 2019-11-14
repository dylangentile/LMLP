#include "include/LMLP.h"



int main(int argc, char const *argv[])
{
	Network myNetwork;
	initializeNetwork(3, &myNetwork, 2, 2, 1);
	
	double 	x0[2] = {0.0, 0.0},
			x1[2] = {1.0, 0.0},
			x2[2] = {0.0, 1.0},
			x3[2] = {1.0, 1.0};
	double 	y0[1] = {0.0},
			y1[1] = {1.0},
			y2[1] = {1.0},
			y3[1] = {0.0};


	insertTruth(&myNetwork, x0, y0, 2, 1);
	insertTruth(&myNetwork, x1, y1, 2, 1);
	insertTruth(&myNetwork, x2, y2, 2, 1);
	insertTruth(&myNetwork, x3, y3, 2, 1);

	printNetwork(&myNetwork);

	runNetwork(&myNetwork, 100001, 0.25, 4, 4);
	printNetwork(&myNetwork);


	return 0;
}