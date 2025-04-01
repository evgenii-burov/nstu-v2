#pragma once
#include <iostream>
#include <random>
#include <omp.h>
#include <time.h>
#include <chrono>
#include <fstream>
#include <string>
#include <iomanip>

void printVector(double* vector, const int& n);

void getScalarProductSequential(double* x, double* y, const int& n);

void getScalarProductParallel(double* x, double* y, const int& n, const int& threads_used);

double* getMatrixATimesBSequential(double* a, double* b, const int& n, const int& m, const int& k);

double* getMatrixATimesBParallel(double* a, double* b, const int& n, const int& m, const int& k, const int& threads_used);

void solveLinearSystemSequential(double* u, double* b, const int& n);

void solveLinearSystemParallel(double* u, double* b, const int& n, const int& threads_used);

double* getTransposedMatrix(double* a, const int& n, const int& m);

double* getMatrixATimesBTransposedParallel(double* a, double* b, const int& n, const int& m, const int& k, const int& threads_used);

double* getMatrixATimesBTransposedSequential(double* a, double* b, const int& n, const int& m, const int& k);