#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#pragma warning(disable:4996)

//typedef unsigned long long size_t;
#define EPSILON 1e-8

#define True 1
#define False 0

#define MATRIX 0
#define VECTOR 1


typedef struct index
{
	size_t i;
	size_t j;

} index;

typedef struct code
{
	int dmode;
	int emode;

}code;

code mode;

size_t lss_memsize_09_03(int n);

int lss_09_03(int n, double* A, double* B, double* X, double* tmp);

index max_elem(double* A, int len, size_t start);

void change_row(double* A, double* B, double* tmp, int len, size_t i_1, size_t i_2);

void change_col(double* A, double* tmp, int len, size_t j_1, size_t j_2);

void forward(double* A, double* B, int len, size_t start);

int backward(double* A, double* B, double* X, double* tmp, int len);

void init_tmp(double* tmp, int len);

void check_accuracy(double* A, double* B, double* X, int len);

void print_mode(double* X, int len);

void print_mv(double* A, double* B, int len);

double change_matrix(double* A, double* B, double* tmp, int n, size_t start);

void print_arr(double* arr, int len, int flag);

