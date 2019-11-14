#include "error.h"
#include <stdlib.h>
#include <stdio.h>

void error()
{
	printf("ERROR!\n");
	abort();
}