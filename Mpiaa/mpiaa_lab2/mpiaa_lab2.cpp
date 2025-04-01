#include "functions.h"

int main(int argc, char** argv)
{
    int n = 2500;
    int m = 2500, k = 2500;

    std::cout << "N: " << n << '\t' << "M: " << m << '\t' << "K: " << k << '\n';
    std::cout << std::setprecision(15);

    // №1
    //double* x = new double[n];
    //double* y = new double[n];
    //for (int i = 0, j = 0; i < n; i++, j++)
    //{
    //    if (j > 10)
    //        j = 0;
    //    x[i] = j;
    //    y[i] = 10 - j;
    //}
    //getScalarProductSequential(x,y,n);
    //getScalarProductParallel(x, y, n, 2);
    //getScalarProductParallel(x, y, n, 12);
    //delete[] x;
    //delete[] y;

    // №2
    //double* a = new double[n*m];
    //double* b = new double[m*k];
    //for (int i = 0, j = 0; i < n*m; i++, j++)
    //{
    //    if (j > 10)
    //        j = 0;
    //    a[i] = j;
    //}
    //for (int i = 0, j = 0; i < m * k; i++, j++)
    //{
    //    if (j > 10)
    //        j = 0;
    //    b[i] = 10 - j;
    //}
    //double* a_times_b = getMatrixATimesBSequential(a, b, n, m, k);
    //delete[] a_times_b;
    //a_times_b = getMatrixATimesBParallel(a, b, n, m, k, 2);
    //delete[] a_times_b;
    //a_times_b = getMatrixATimesBParallel(a, b, n, m, k, 4);
    //delete[] a_times_b;
    //a_times_b = getMatrixATimesBParallel(a, b, n, m, k, 8);
    //delete[] a_times_b;
    //a_times_b = getMatrixATimesBParallel(a, b, n, m, k, 12);
    //delete[] a_times_b;
    //delete[] a;
    //delete[] b;

    // №3
    //double* u = new double[n * n];
    //for (int i = 0; i < n; i++)
    //{
    //    for (int j = 0; j < i; j++)
    //    {
    //        u[i * n + j] = 0;
    //    }
    //    u[i * n + i] = 10;
    //    for (int j = i+1; j < n; j++)
    //    {
    //        u[i * n + j] = 1;
    //    }
    //}
    //double* x = new double[n];
    //for (int i = 0, j = 0; i < n; i++, j++)
    //{
    //    if (j > 10)
    //        j = 0;
    //    x[i] = j;
    //}
    //double vector_x_norm = 0;
    //for (int i = 0; i < n; i++)
    //{
    //    vector_x_norm += pow(x[i], 2);
    //}
    //vector_x_norm = sqrt(vector_x_norm);
    //std::cout << "\nVector x norm: " << vector_x_norm << "\n";
    //std::ofstream output;
    //output.open("vector_x.txt");
    //for (int i = 0; i < n; i++)
    //{
    //    output << x[i] << '\t';
    //}
    //output.close();
    //double* b = getMatrixATimesBSequential(u, x, n, n, 1);
    ////solveLinearSystemSequential(u, b, n);
    //solveLinearSystemParallel(u, b, n, 12);
    //delete[] u;
    //delete[] x;
    //delete[] b;

    //№4
    //double* a = new double[n*m];
    //double* b = new double[m*k];
    //for (int i = 0, j = 0; i < n*m; i++, j++)
    //{
    //    if (j > 10)
    //        j = 0;
    //    a[i] = j;
    //}
    //for (int i = 0, j = 0; i < m * k; i++, j++)
    //{
    //    if (j > 10)
    //        j = 0;
    //    b[i] = 10 - j;
    //}
    //double* b_t = getTransposedMatrix(b, m, k);

    //double* a_times_b = getMatrixATimesBParallel(a, b, n, m, k, 2);
    //delete[] a_times_b;
    //double* a_times_b_transposed = getMatrixATimesBTransposedParallel (a, b_t, n, m, k, 2);
    //a_times_b = getMatrixATimesBParallel(a, b, n, m, k, 4);
    //a_times_b_transposed = getMatrixATimesBTransposedParallel(a, b_t, n, m, k, 4);
    //delete[] a_times_b;
    //delete[] a_times_b_transposed;
    //a_times_b = getMatrixATimesBParallel(a, b, n, m, k, 8);
    //a_times_b_transposed = getMatrixATimesBTransposedParallel(a, b_t, n, m, k, 8);
    //delete[] a_times_b;
    //delete[] a_times_b_transposed;
    //a_times_b = getMatrixATimesBParallel(a, b, n, m, k, 12);
    //a_times_b_transposed = getMatrixATimesBTransposedParallel(a, b_t, n, m, k, 12);
    //delete[] a_times_b;
    //delete[] a_times_b_transposed;
    //delete[] a;
    //delete[] b;
    //delete[] b_t;
}