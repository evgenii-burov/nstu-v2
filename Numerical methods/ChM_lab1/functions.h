#pragma once

using precision = double;

void readParameters(int& n, int& p, int& matrix_type, int& k);

void createMatrixFile(int& n, int& p, int& matrix_type, int& k);

void createVectorFile(precision** al, const int& n, const int& matrix_type);

void readMatrix(precision** al, const int& n, const int& p, const int& matrix_type);

void readVector(precision*& b, const int& n, const int& matrix_type);

//not used
void manageInput(precision**& al, precision*& b, int& n, int& p);

//not used
void multiplyAlByVector(precision** al, const int& n);

void calculateDecomposition(precision** al, const int& n, const int& p);

void printVariables(precision** al, precision* b, const int& n, const int& p);

void solveForX(precision** al, precision* &b, const int& n, const int& p);

void printDenseMatrix(precision** al, const int& n, const int& p);

void solveUsingGauss(precision** al, precision*& b, const int& n, const int& p);