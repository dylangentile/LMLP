#include "matrix.h"
#include "error.h"
#include <stdlib.h>

int intializeMatrix(Matrix* theMatrix, int m, int n)
{
	theMatrix->j = m;
	theMatrix->k = n;
	//todo: malloc errors
	theMatrix->array = calloc(m, sizeof(double*));
	for(unsigned int i = 0; i < m; i++)
	{
		theMatrix->array[i] = calloc(n, sizeof(double));
	}

	//should be set to zero
	/*for(unsigned int i = 0; i < m; i++)
	{
		for(unsigned int w = 0; w < k; w++)
		{
			if(f != NULL)
			{
				theMatrix->array[i][w] = f();
			}
		}
	}*/

	return 1;

}

int deleteMatrix(Matrix* theMatrix)
{
	for(unsigned int x = 0; x < theMatrix->j; x++)
	{
		free(theMatrix->array[x]);
	}
	free(theMatrix->array);
	return 1;
}

void randomizeMatrixValues(Matrix* theMatrix, double (*generator)(void))
{
	for(int m = 0; m < theMatrix->j; m++)
	{
		for(int n = 0; n < theMatrix->k; n++)
		{
			theMatrix->array[m][n] = generator();
		}
	}
}

void applyFunctionToMatrix(Matrix* theMatrix, double (*f)(double))
{
	for(int m = 0; m < theMatrix->j; m++)
	{
		for(int n = 0; n < theMatrix->k; n++)
		{
			theMatrix->array[m][n] = f(theMatrix->array[m][n]);
		}
	}
}

Matrix matrixMultiply(Matrix* matrixA, Matrix* matrixB)
{
	if(matrixA->k != matrixB->j)
	{
		error();
		//return *NULL;
	}

	Matrix result;

	if(!intializeMatrix(&result, matrixA->j, matrixB->k))
	{
		error();
		//return *NULL;
	}

	for(unsigned int m = 0; m < result.j; m++)
	{
		for(unsigned int n = 0; n < result.k; n++)
		{
			result.array[m][n] = 0; //they should already be set to 0 because of calloc but...
			for(int i = 0; i < matrixA->k; i++)
			{
				result.array[m][n] += matrixA->array[m][i] * matrixB->array[i][n];
			}
		}
	}
	
	return result;
}



Matrix matrixAdd(Matrix* matrixA, Matrix* matrixB)
{
	if(matrixA->j != matrixB->j || matrixA->k != matrixB->k)
	{
		error();
	}

	Matrix result;
	if(!intializeMatrix(&result, matrixA->j, matrixA->k))
	{
		error();
	}

	for(unsigned int x = 0; x < result.j; x++)
	{
		for(unsigned int y = 0; y < result.k; y++)
		{
			result.array[x][y] = matrixA->array[x][y] + matrixB->array[x][y];
		}
	}

	return result;
}
//A = B
void matrixCopy(Matrix* matrixA, Matrix* matrixB)
{
	if(matrixA->j != matrixB->j || matrixA->k != matrixB->k)
	{
		error();
	}

	for(unsigned int x = 0; x < matrixA->j; x++)
	{
		for(unsigned int y = 0; y < matrixA->k; y++)
		{
			matrixA->array[x][y] = matrixB->array[x][y];
		}
	}
}



