#include "lss_09_03.h"


size_t lss_memsize_09_03(int n)
{
	return n * sizeof(double);
}


int lss_09_03(int n, double* A, double* B, double* X, double* tmp)
{
	index cordinatsofmax;

	init_tmp(tmp, n);

	if (mode.dmode == True)
	{
		printf("\n(0) Starting calculations\n");
		print_mv(A, B, n);
		for (size_t iter = 0; iter < n-1; iter++)
		{
			printf("(1)Finding max element in the current iteration ...\n");
			cordinatsofmax = max_elem(A, n, iter);
			printf("(2)Maximum element in the current iteration: %lf\n", A[cordinatsofmax.i * n + cordinatsofmax.j]);

			if (fabs(A[cordinatsofmax.i * n + cordinatsofmax.j]) > EPSILON)
			{
				printf("(3)Changing row ...\n");
				change_row(A, B, tmp, n, iter, cordinatsofmax.i);
				print_mv(A, B, n);

				printf("(4)Changing column ...\n");
				change_col(A, tmp, n, iter, cordinatsofmax.j);
				print_mv(A, B, n);

				printf("Executing forward method of Gauss ...\n");
				forward(A, B, n, iter);
				print_mv(A, B, n);

				if (iter != n - 1)
					printf("Next iteration ... \n");

			}
			else break;
	
		}
		printf("Executing backward method of Gauss ...\n");
		return backward(A, B, X, tmp, n);
	}
	else
	{
		for (size_t iter = 0; iter < n - 1; iter++)
		{
			cordinatsofmax = max_elem(A, n, iter);

			if (fabs(A[cordinatsofmax.i * n + cordinatsofmax.j]) > EPSILON)
			{
				change_row(A, B, tmp, n, iter, cordinatsofmax.i);

				change_col(A, tmp, n, iter, cordinatsofmax.j);

				forward(A, B, n, iter);

			}
			else break;
		}
		return backward(A, B, X, tmp, n);
	}
}

void forward(double* A, double* B, int len, size_t start)
{
	for (size_t i = start; i < len-1; i++)
	{
		
		const double coeff = A[(i+1) * len + start] / A[start * len + start];

		for (size_t j = start; j < len; j++)
		{

			A[(i + 1) * len + j] = A[(i + 1) * len + j] - A[start * len + j] * coeff;
		}

		B[i+1] = B[i+1] - B[start] * coeff;
	}

}


int backward(double* A, double* B, double* X,double* tmp, int len)
{
	const double  C = 0;

	for (size_t i = len; i-- > 0;)
	{
		for (size_t j = i+1; j < len; j++)
		{
			B[i] = B[i] - A[i*len + j] * X[(int)tmp[i*len+j]];
		}
		if (fabs(B[i]) <= EPSILON && fabs(A[i*len+i]) <= EPSILON)
		{
			X[(int)tmp[i * len + i]] = C;
			continue;
		}
		else if (fabs(B[i]) >= EPSILON && fabs(A[i*len+i]) <= EPSILON)
		{
			return 1;
		}

		X[(int)tmp[i*len+i]] = B[i] / A[i*len + i];
	}

	return 0; 
}


index max_elem(double* A, int len, size_t start)
{
	index answ; answ.i = -1; answ.j = -1;

	double max = 0;
	for (size_t i = start; i < len; i++)
		for (size_t j = start; j < len; j++)
		{
			if (fabs(A[i * len + j]) >= max)
			{
				max = fabs(A[i * len + j]);
				answ.i = i;
				answ.j = j;
			}
		}

	return answ;
}


void change_row(double* A,double* B,double* tmp, int len, size_t i_1, size_t i_2)
{
	// меняем строку i_1 и строку i_2 местами

	if (i_1 != i_2)
	{
		double swap = 0;

		for (size_t row = 0; row < 1; row++)
		{
			for (size_t col = i_1; col < len; col++)
			{
				swap = A[i_1 * len + col];
				A[i_1 * len + col] = A[i_2 * len + col];
				A[i_2 * len + col] = swap;

				swap = tmp[i_1 * len + col];
				tmp[i_1 * len + col] = tmp[i_2 * len + col];
				tmp[i_2 * len + col] = swap;

			}
		}
		swap = B[i_1];
		B[i_1] = B[i_2];
		B[i_2] = swap;

	}
	
}

void change_col(double* A,double* tmp, int len, size_t j_1, size_t j_2)
{
	// меняем столбец j_1 и столбец j_2 местами 

	if (j_1 != j_2)
	{
		double swap = 0;
		
		for (size_t col = 0; col < 1; col++)
		{
			for (size_t row = j_1; row < len; row++)
			{
				swap = A[row * len + j_1];
				A[row * len + j_1] = A[row * len + j_2];
				A[row * len + j_2] = swap;

				swap = tmp[row * len + j_1];
				tmp[row * len + j_1] = tmp[row * len + j_2];
				tmp[row * len + j_2] = swap;

			}

		}
	}

}


void init_tmp(double* tmp, int len)
{
	for (size_t i = 0; i < len; i++)
	{
		for (size_t j = 0; j < len; j++)
		{
			tmp[i * len + j] = j;
		}
	}
}


double change_matrix(double* A, double* B, double* tmp, int n, size_t start)
{
	index cor = max_elem(A, n, start);
	double maxzero = A[cor.i * n + cor.j];
	change_row(A, B, tmp, n, start, cor.i);

	change_col(A, tmp, n, start, cor.j);

	return maxzero;
}


void check_accuracy(double* A, double* B, double* X, int len)
{
	double acc = 0;
	for (size_t i = 0; i < len; i++)
	{
		double answ = 0.0;
		for (size_t j = 0; j < len; j++)
		{
			answ = answ + A[i * len + j] * X[j];
		}
		acc += fabs(B[i] - answ);

		printf("%lf\n", answ);
	}

	printf("error-rate: %.15g\n", acc / len);
}



void print_mv(double* A, double* B, int len)
{	
	for (size_t i = 0; i < len; i++)
	{
		for (size_t j = 0; j < len; j++)
		{	
			printf("%1.9lf  ", A[i * len + j]);
		}
		printf("   = %1.9lf\n", B[i]);
	}
	printf("\n");

}


void print_arr(double* arr, int len, int flag)
{
	// flag = 0;(prints matrix) --- flag = 1; (prints vector)
	
	if (flag == 0)
	{
		for (size_t i = 0; i < len; i++)
		{
			for (size_t j = 0; j < len; j++)
				printf("%1.9lf ", arr[i * len + j]);
			
			printf("\n");
		}
		printf("\n");
	}
	else if (flag == 1)
	{
		for (size_t i = 0; i < len; i++)
		{
			printf("%1.9lf \n", arr[i]);
		}
		printf("\n");
	}
	else  printf("Incorrect type");
}


void print_mode(double* X, int len)
{
	for (size_t i = 0; i < len; i++)
	{
		for (size_t j = 0; j < len; j++)
		{
			if (i == j)
			{
				printf("%1.9lf  ", 1.0);
			}
			else
			{
				printf("%1.9lf  ", 0.0);
			}
		}
		printf("  =  %1.9lf\n", X[i]);
	}
	printf("\n");
}