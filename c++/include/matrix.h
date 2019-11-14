#pragma once

class Matrix
{
public:
	Matrix(unsigned m, unsigned n, double(*f)(void) = nullptr);
	~Matrix();
	unsigned w(){ return j; }
	unsigned z(){ return k; }
	double& get(unsigned m, unsigned n);

	Matrix applyFunctionToAll(double (*f)(double));
	const Matrix operator*(Matrix& that)
	{


		if(this->k != that.j)
		{
			throw "bad matrix multiplication! Matricies k and j don't match";
		}



		Matrix result(this->j, that.k);

		for(unsigned m = 0; m < result.j; m++)
		{
			for(unsigned n = 0; n < result.k; n++)
			{
				result.array[m][n] = 0;
				for(int i = 0; i < this->k; i++)
				{
					result.array[m][n] += this->array[m][i] * that.array[i][n];
				}
			}
		}


		return result;
	}
	const Matrix operator+(const Matrix& input)
	{
		if(this->j != input.j || this->k != input.k)
		{
			throw "poo poo pee pee";
		}
		
		Matrix result(input.j, input.k);
		for(unsigned x = 0; x < this->j; x++)
		{
			for(unsigned y = 0; y < this->k; y++)
			{
				result.array[x][y] = this->array[x][y] + input.array[x][y];
			}
		}

		return result;

	}

	const Matrix& operator=(const Matrix& input)
	{
		if(this->j != input.j || this->k != input.k)
		{
			throw "poo poo pee pee";
		}

		for(unsigned x = 0; x < this->j; x++)
		{
			for(unsigned y = 0; y < this->k; y++)
			{
				this->array[x][y] = input.array[x][y];
			}
		}

		return *this;
	}
	
	double* operator[](unsigned long i)
	{
		/*if(k == 1)
		{
			return &array[i][0];
		}
		else*/
		{
			return array[i];
		}
	}

	void set(unsigned m, unsigned n, double value);
	void size(unsigned& m, unsigned& n);
	void print();
	double** getRawArray();
	
private:
	double** array;
	unsigned j, k; //row it, column it
};