#include "network.h"
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
	vector<unsigned int> layerSizes = {2, 3, 1};
	Network* theNetwork = new Network(layerSizes);
	
	theNetwork->print();

	vector<double> x0 = {0.0, 0.0};
	vector<double> y0 = {0.0};
	vector<double> x1 = {0.0, 1.0};
	vector<double> y1 = {1.0};
	vector<double> x2 = {1.0, 0.0};
	vector<double> y2 = {1.0};
	vector<double> x3 = {1.0, 1.0};
	vector<double> y3 = {0.0};
	
	theNetwork->passTruth(x0, y0);
	theNetwork->passTruth(x1, y1);
	theNetwork->passTruth(x2, y2);
	theNetwork->passTruth(x3, y3);

	theNetwork->run(100001, .25, 4, 50);

	theNetwork->print();


	return 0;
}