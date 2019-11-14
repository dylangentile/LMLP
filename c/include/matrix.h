#ifndef MATRIX_H
#define MATRIX_H


typedef struct {
	unsigned int j, k;
	double** array;
} Matrix;

int intializeMatrix(Matrix* theMatrix, int m, int n);
int deleteMatrix(Matrix* theMatrix);

void randomizeMatrixValues(Matrix* theMatrix, double (*generator)(void));
void applyFunctionToMatrix(Matrix* theMatrix, double (*f)(double));

Matrix matrixMultiply(Matrix* matrixA, Matrix* matrixB);
Matrix matrixAdd(Matrix* matrixA, Matrix* matrixB);
void matrixCopy(Matrix* matrixA, Matrix* matrixB); //A = B;




#endif //MATRIX_H