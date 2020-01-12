#include "function.h"
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#define UNIX_SYSTEM
#endif
#if defined(_WIN32) || defined(WIN32)
#define WIN_SYSTEM
#endif

#include <cmath>
#include <cstdlib>
#include <ctime>
#ifdef UNIX_SYSTEM
#include <unistd.h>
#endif

#ifdef WIN_SYSTEM
#include <process.h>
#endif



unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

/*class Generator
{
public:
	Generator() : seed(mix(clock(), time(NULL), getpid()))
	{
		srand(seed);
	}
	unsigned long rnd()
	{
		return rand();
	}

	const unsigned long seed;

};*/


//Generator *theGenerator = new Generator;

void seedGenerator()
{
	//srand(5);
#ifdef UNIX_SYSTEM
	srand(mix(clock(), time(NULL), getpid()));
#endif
#ifdef WIN_SYSTEM
	srand(mix(time(NULL), clock(), _getpid()));
#endif
}

double sigmoid(double x)
{
	return 1.0/(1.0+exp(-1.0*x));
}

double dsigmoid(double x)
{
	double f = sigmoid(x);
	return f*(1-f);
}

double getRand()
{
	//return 0.75;
	return (double)rand()/RAND_MAX*2.0-1.0;
}


unsigned long random_0_to_n(unsigned long n)
{
	if(n == 0)
		return 0;
	else
		return rand()%n;
}
