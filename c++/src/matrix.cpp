#include "matrix.h"
#include <cstdio>

using namespace std;

Matrix::Matrix(unsigned m, unsigned n, double(*f)(void))
{
	j = m;
	k = n;

	array = new double*[j];

	for(unsigned i = 0; i < j; i++)
	{
		array[i] = new double[k];

	}

	for(unsigned i = 0; i < j; i++)
	{
		for(unsigned w = 0; w < k; w++)
		{
			if(f != nullptr)
			{
				array[i][w] = f();
			}
			else
			{
				array[i][w] = 0;
			}
		}
	}



}

Matrix::Matrix(const Matrix &i_matrix)

{

	j = i_matrix.j;

	k = i_matrix.k;



	array = new double*[j];

	for (unsigned i = 0; i < j; i++)

		array[i] = new double[k];





	for (unsigned x = 0; x < this->j; x++)

	{

		for (unsigned y = 0; y < this->k; y++)

		{

			this->array[x][y] = i_matrix.array[x][y];

		}

	}





}


double& 
Matrix::get(unsigned m, unsigned n)
{
	return array[m][n];
}

double**
Matrix::getRawArray()
{
	return array;
}

void
Matrix::set(unsigned m, unsigned n, double value)
{
	array[m][n] = value;
}

void
Matrix::size(unsigned& m, unsigned& n)
{
	m = j;
	n = k;
}

Matrix::~Matrix()
{
	for(unsigned i = 0; i < j; i++)
	{
		delete array[i];
	}
	delete array;
}

Matrix 
Matrix::applyFunctionToAll(double (*f)(double))
{
	Matrix ans(j, k);
	for(int x = 0; x < j; x++)
	{
		for(int y = 0; y < k; y++)
		{
			ans.array[x][y] = f(array[x][y]);
		}
	}
	return ans;
}

void
Matrix::print()
{
	for(int x = 0; x < j; x++)
	{
		for(int y = 0; y < k; y++)
		{
			printf("%4.3f\t", array[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}