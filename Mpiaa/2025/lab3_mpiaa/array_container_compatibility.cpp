#include "header.h"

double Sum(int n, double* a)
{
    double s = 0;

    for (int i = 0; i < n; i++)
    {
        s += a[i];
    }

    return s;
}
//-----------------------------------------------------------------------------
double SumVectorVector(std::vector<std::vector<double> >& a)
{
    double s = 0;

    int sz1 = a.size();
    for (int i = 0; i < sz1; i++)
    {
        int sz2 = a[i].size();
        for (int j = 0; j < sz2; j++)
        {
            s += a[i][j];
        }
    }

    return s;
}

//-----------------------------------------------------------------------------
void TestSumMatrix(int n, int m)
{
    std::vector<std::vector<double> > a;
    a.resize(n, std::vector<double>(m, 1));

    std::cout << SumVectorVector(a) << '\t' << Sum(n * m, &a[0][0]) << std::endl;
}
//-----------------------------------------------------------------------------
void array_container_compatibility()
{
    std::vector<int> values_of_n = { 1,1,2,2,10 };
    std::vector<int> values_of_m = { 1,10,1,10,10 };
    std::cout << "n\tm\tvector\tarray\n";
    for (int i = 0; i < 5; i++)
    {
        int n = values_of_n[i];
        int m = values_of_m[i];
        std::cout << n << "\t" << m << "\t";
        TestSumMatrix(n, m);
        std::cout << "\t\n";
    }
}