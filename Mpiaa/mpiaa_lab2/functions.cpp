#include "functions.h"

void printVector(double* vector, const int& n)
{
    for (long long i = 0; i < n; i++)
    {
        std::cout << vector[i] << ' ';
    }
    std::cout << '\n';
}

void getScalarProductSequential(double* x, double* y, const int& n)
{
    auto t1_chrono = std::chrono::high_resolution_clock::now();
    clock_t t1_time_h = clock();
    double scalar_product = 0;
    for (long long i = 0; i < n; i++)
    {
        scalar_product += x[i] * y[i];
    }
    auto t2_chrono = std::chrono::high_resolution_clock::now();
    clock_t t2_time_h = clock();
    auto time_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_chrono - t1_chrono).count();
    double time_time_h = (double)(t2_time_h - t1_time_h) / CLOCKS_PER_SEC;
    std::cout << "Scalar product, sequential: " << scalar_product;
    std::cout << "\nSequential runtime (chrono, milliseconds): " << time_chrono / pow(10, 6);
    std::cout << "\nSequential runtime (time.h, milliseconds): " << time_time_h << "\n\n";
}

void getScalarProductParallel(double* x, double* y, const int& n, const int& threads_used)
{
    auto t1_chrono = std::chrono::high_resolution_clock::now();
    clock_t t1_time_h = clock();
    double scalar_product = 0;
#pragma omp parallel num_threads(threads_used)
    {
#pragma omp for reduction(+:scalar_product)
        for (int i = 0; i < n; i++)
        {
            scalar_product += x[i] * y[i];
        }
    }
    auto t2_chrono = std::chrono::high_resolution_clock::now();
    clock_t t2_time_h = clock();
    auto time_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_chrono - t1_chrono).count();
    double time_time_h = (double)(t2_time_h - t1_time_h) / CLOCKS_PER_SEC;
    std::cout << "Scalar product, parallel: " << scalar_product;
    std::cout << "\nParallel runtime (chrono, milliseconds): " << time_chrono / pow(10, 6);
    std::cout << "\nParallel runtime (time.h, milliseconds): " << time_time_h;
    std::cout << "\nThreads used in parallel: " << threads_used << "\n\n";
}

double* getMatrixATimesBSequential(double* a, double* b, const int& n, const int& m, const int& k)
{
    double matrix_norm = 0;
    double* a_times_b = new double[n * k];
    auto t1_chrono = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            a_times_b[i * k + j] = 0;
            for (int l = 0; l < m; l++)
            {
                a_times_b[i * k + j] += a[i * m + l] * b[l * k + j];
            }
            //matrix_norm += pow(a_times_b[i * k + j], 2);
        }
    }
    auto t2_chrono = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            matrix_norm += pow(a_times_b[i * k + j], 2);
        }
    }
    matrix_norm = sqrt(matrix_norm);
    

    auto time_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_chrono - t1_chrono).count();
    std::cout << "Matrix norm, sequential: " << matrix_norm;
    std::cout << "\nSequential runtime (chrono, milliseconds): " << time_chrono / pow(10, 6) << "\n\n";
    return a_times_b;
}

double* getMatrixATimesBParallel(double* a, double* b, const int& n, const int& m, const int& k, const int& threads_used)
{
    double matrix_norm = 0;
    double* a_times_b = new double[n * k];
    auto t1_chrono = std::chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(threads_used)
    {
#pragma omp for reduction(+:matrix_norm)
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < k; j++)
            {
                a_times_b[i * k + j] = 0;
                for (int l = 0; l < m; l++)
                {
                    a_times_b[i * k + j] += a[i * m + l] * b[l * k + j];
                }
                //matrix_norm += pow(a_times_b[i * k + j], 2);
            }
        }
    }
    auto t2_chrono = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            matrix_norm += pow(a_times_b[i * k + j], 2);
        }
    }
        matrix_norm = sqrt(matrix_norm);

    auto time_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_chrono - t1_chrono).count();
    std::cout << "Matrix norm, parallel: " << matrix_norm;
    std::cout << "\nParallel runtime (chrono, milliseconds): " << time_chrono / pow(10, 6);
    std::cout << "\nThreads used in parallel: " << threads_used << "\n\n";
    return a_times_b;
}

void solveLinearSystemSequential(double* u, double* b, const int& n)
{
    double* y = new double[n];
    double sum_over_k = 0;
    auto t1_chrono = std::chrono::high_resolution_clock::now();
    for (int i = n - 1; i >= 0; i--)
    {
        sum_over_k = 0;
        for (int k = i + 1; k < n; k++)
        {
            sum_over_k += u[i * n + k] * y[k];
        }
        y[i] = (b[i] - sum_over_k) / u[i * n + i];
    }
    auto t2_chrono = std::chrono::high_resolution_clock::now();

    auto time_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_chrono - t1_chrono).count();
    double vector_y_norm = 0;
    for (int i = 0; i < n; i++)
    {
        vector_y_norm += pow(y[i], 2);
    }
    vector_y_norm = sqrt(vector_y_norm);
    std::cout << "\nSequential runtime (chrono, milliseconds): " << time_chrono / pow(10, 6) << "\n\n";
    std::cout << "\nVector y norm (sequential): " << vector_y_norm << "\n";
    std::ofstream output;
    output.open("vector_y_sequential.txt");
    for (int i = 0; i < n; i++)
    {
        output << y[i] << '\t';
    }
    output.close();

    delete[] y;
}

void solveLinearSystemParallel(double* u, double* b, const int& n, const int& threads_used)
{
    double* y = new double[n];
    double sum_over_k = 0;
    auto t1_chrono = std::chrono::high_resolution_clock::now();

    for (int i = n - 1; i >= 0; i--)
    {
        sum_over_k = 0;
#pragma omp parallel for reduction(+:sum_over_k) num_threads(threads_used)
        for (int k = i + 1; k < n; k++)
        {
            sum_over_k += u[i * n + k] * y[k];
        }
        y[i] = (b[i] - sum_over_k) / u[i * n + i];
    }
    auto t2_chrono = std::chrono::high_resolution_clock::now();

    auto time_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_chrono - t1_chrono).count();
    double vector_y_norm = 0;
    for (int i = 0; i < n; i++)
    {
        vector_y_norm += pow(y[i], 2);
    }
    vector_y_norm = sqrt(vector_y_norm);
    std::cout << "\nParallel runtime (chrono, milliseconds): " << time_chrono / pow(10, 6);
    std::cout << "\nVector y norm (sequential): " << vector_y_norm << "\n";
    std::cout << "\nThreads used in parallel: " << threads_used << "\n\n";

    std::ofstream output;
    output.open("vector_y_parallel.txt");
    for (int i = 0; i < n; i++)
    {
        output << y[i] << '\t';
    }
    output.close();

    delete[] y;
}

double* getTransposedMatrix(double* a, const int& n, const int& m)
{
    double* a_transposed = new double[m * n];
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            a_transposed[i * n + j] = a[j * m + i];
        }
    }
    return a_transposed;
}

double* getMatrixATimesBTransposedParallel(double* a, double* b, const int& n, const int& m, const int& k, const int& threads_used)
{
    double matrix_norm = 0;
    double* a_times_b = new double[n * k];
    auto t1_chrono = std::chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(threads_used)
    {
#pragma omp for reduction(+:matrix_norm)
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < k; j++)
            {
                a_times_b[i * k + j] = 0;
                for (int l = 0; l < m; l++)
                {
                    a_times_b[i * k + j] += a[i * m + l] * b[j * m + l];
                }
                //matrix_norm += pow(a_times_b[i * k + j], 2);
            }
        }
    }
    auto t2_chrono = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            matrix_norm += pow(a_times_b[i * k + j], 2);
        }
    }
    matrix_norm = sqrt(matrix_norm);

    auto time_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_chrono - t1_chrono).count();
    std::cout << "Matrix norm, transposed, parallel: " << matrix_norm;
    std::cout << "\nParallel runtime (chrono, milliseconds): " << time_chrono / pow(10, 6);
    std::cout << "\nThreads used in parallel: " << threads_used << "\n\n";
    return a_times_b;
}

double* getMatrixATimesBTransposedSequential(double* a, double* b, const int& n, const int& m, const int& k)
{
    double matrix_norm = 0;
    double* a_times_b = new double[n * k];
    auto t1_chrono = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < k; j++)
            {
                a_times_b[i * k + j] = 0;
                for (int l = 0; l < m; l++)
                {
                    a_times_b[i * k + j] += a[i * m + l] * b[j * m + l];
                }
                //matrix_norm += pow(a_times_b[i * k + j], 2);
            }
        }

        //for (int i = 0; i < n; i++)
        //{
        //    for (int j = 0; j < k; j++)
        //    {
        //        a_times_b[i * k + j] = 0;
        //        for (int l = 0; l < m; l++)
        //        {
        //            a_times_b[i * k + j] += a[i * m + l] * b[l * k + j];
        //        }
        //        //matrix_norm += pow(a_times_b[i * k + j], 2);
        //    }
        //}

    auto t2_chrono = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            matrix_norm += pow(a_times_b[i * k + j], 2);
        }
    }
    matrix_norm = sqrt(matrix_norm);

    auto time_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_chrono - t1_chrono).count();
    std::cout << "Matrix norm, transposed, sequential: " << matrix_norm;
    std::cout << "\nSequential runtime (chrono, milliseconds): " << time_chrono / pow(10, 6) << "\n\n";
    return a_times_b;
}