#include "functions.h"

void getSampleFrequencies(double*& sample, double*& frequencies, const int& N, const int& k)
{
    double x_min = DBL_MAX;
    double x_max = DBL_MIN;
    for (int i = 0; i < N; i++)
    {
        if (sample[i] < x_min)
            x_min = sample[i];
        if (sample[i] > x_max)
            x_max = sample[i];
    }
    double R = x_max - x_min;
    double h = R / k;
    int interval_index = 0;
    for (int i = 0; i < k; i++)
    {
        frequencies[i] = 0;
    }
    for (int i = 0; i < N; i++)
    {
        interval_index = int((sample[i] - x_min) / h); //(sample[i] - x_min)/h = 0..k
        interval_index -= int(((sample[i] - x_min) / h) / k);
        frequencies[interval_index]++;
    }
    //int counter = 0;
    //int freq_i = 0;
    //for (int i = 0; i < k; i++)
    //{
    //    freq_i = frequencies[i];
    //    for (int j = 0; j < freq_i; j++)
    //    {
    //        function[counter] = freq_i / (N * h);
    //        counter++;
    //    }
    //}
    //for (int i = 0; i < N; i++)
    //{
    //    interval_index = int((sample[i] - x_min) / h);
    //    interval_index -= int(((sample[i] - x_min) / h) / k);;//interval_index -= int(((sample[i] - x_min) / h) / k);
    //    function[i] = frequencies[interval_index] / (N * h);
    //}
}

double calculateEmpiricSampleCharacteristic(double*& sample, const int& N, const char& value_desired)
{
    double d = 0;
    double m = 0;
    double a = 0;
    double e = 0;
    for (int i = 0; i < N; i++)
    {
        m += sample[i];
    }
    m = m / N;
    for (int i = 0; i < N; i++)
    {
        d += pow((sample[i] - m), 2);
    }
    d = d / N;
    for (int i = 0; i < N; i++)
    {
        a += pow((sample[i] - m), 3);
    }
    a = a / (N * pow(d, 1.5));
    for (int i = 0; i < N; i++)
    {
        e += pow((sample[i] - m), 4);
    }
    e = e / (N * pow(d, 2)) - 3;
    switch (value_desired)
    {
    default:
        std::cout << "Empiric values:\nm: " << m << "\nd: " << d << "\na: " << a << "\ne: " << e << "\n\n";
        return DBL_MAX;
    case'M':
        return m;
    case'D':
        return d;
    case'A':
        return a;
    case'E':
        return e;
    }
}

double calculateTheoreticStandartCharacteristic(const double& nu, const double& mu, const double& lambda,\
    const char& value_desired, double x)
{
    const double a = PI * tan(PI * nu / 2);
    const double K = sin(PI * nu) * (1 / PI + PI / (a * a)) + nu;
    const double C = (sin(PI * nu) / PI + nu) / K; // C := P
    double M, D, A, E;
    M = mu; //mu lambda transformation
    D = 2 * (pow(PI * nu, 3) / 6 + (pow(PI * nu, 2) / 2 - 1) * sin(PI * nu) + PI * nu * cos(PI * nu)) \
        / (pow(PI, 3) * K) + 2 * pow(cos(PI * nu / 2), 2) * (2 / (a * a) + 2 * nu / a + nu * nu) / (a * K);
    D *= pow(lambda, 2); //mu lambda transformation
    A = 0;
    E = 4 * ((pow(PI * nu, 5) / 20) + (sin(PI * nu) * (pow(PI * nu, 4) / 4 - 3 * pow(PI * nu, 2) + 6)) + (cos(PI * nu) * (pow(PI * nu, 3) - 6 * PI * nu))) / \
        (pow(D, 2) * pow(PI, 5) * K) + (2 * pow(cos(PI * nu / 2), 2) / (pow(D, 2) * K)) * \
        (24 / pow(a, 5) + 24 * nu / pow(a, 4) + 12 * pow(nu, 2) / pow(a, 3) + 4 * pow(nu, 3) / pow(a, 2) + pow(nu, 4) / pow(a, 1)) - 3;
    E = (E+3)*pow(lambda,4) - 3; //mu lambda transformation
    switch (value_desired)
    {
    default:
        return FLT_MAX;
    case 'K':
        return K;
    case 'F':
    {
        x = (x-mu)/lambda;
        double f_of_xnu = 0;
        if (abs(x) <= nu)
        {
            f_of_xnu = pow(cos(PI * x / 2), 2);
        }
        else
        {
            f_of_xnu = pow(cos(PI * nu / 2), 2) * exp(-1 * a * (abs(x) - nu));
        }
        return f_of_xnu / (K*lambda);
    }
    case 'P':
        return C;
    case 'M':
        return M;
    case 'D':
        return D;
    case 'A':
        return A;
    case 'E':
        return E;
    }
}

double calculateTheoreticMixCharacteristic(const double& nu1, const double& mu1, const double& lambda1, \
    const double& nu2, const double& mu2, const double& lambda2, const double& p,\
    const char& value_desired, double x)
{
    double M, D, A, E, F;
    double m1,m2,d1, d2, e1, e2;
    double a_first_part, a_second_part, e_first_part, e_second_part;
    d1 = calculateTheoreticStandartCharacteristic(nu1, mu1, lambda1, 'D');
    d2 = calculateTheoreticStandartCharacteristic(nu2, mu2, lambda2, 'D');
    e1 = calculateTheoreticStandartCharacteristic(nu1, mu1, lambda1, 'E');
    e2 = calculateTheoreticStandartCharacteristic(nu2, mu2, lambda2, 'E');
    //e1 = calculateTheoreticStandartCharacteristic(nu1, 0, 1, 'E');
    //e2 = calculateTheoreticStandartCharacteristic(nu2, 0, 1, 'E');
    m1 = mu1;
    m2 = mu2;
    //p1 = 1-p; p2=p;
    M = (1 - p) * m1 + p * m2;
    //D=p_i(m_i^2 + d_i) - M^2
    D = (1 - p) * (pow(m1,2)+d1) + p * (pow(m2, 2) + d2) - pow(M,2);

    a_first_part = pow((m1 - M), 3) + 3 * (m1 - M) * d1;
    a_second_part = pow((m2 - M), 3) + 3 * (m2 - M) * d2;
    A = ((1 - p) * a_first_part + p * a_second_part) / pow(D, 1.5);

    e_first_part = pow((m1 - M), 4) + 6 * pow((m1 - M), 2) * d1 + pow(d1, 2) * (e1 + 3);
    e_second_part = pow((m2 - M), 4) + 6 * pow((m2 - M), 2) * d2 + pow(d2, 2) * (e2 + 3);
    E = (((1 - p) * e_first_part + p * e_second_part) / pow(D, 2)) - 3;
    F = (1 - p) * calculateTheoreticStandartCharacteristic(nu1, mu1, lambda1, 'F', x) \
        + p * calculateTheoreticStandartCharacteristic(nu2, mu2, lambda2, 'F', x);
    switch (value_desired)
    {
    default:
        return FLT_MAX;
        break;
   
    case 'F':
        return F;
    case 'M':
        return M;
    case 'D':
        return D;
    case 'A':
        return A;
    case 'E':
        return E;
    }
}

void UniformDistribution(double*& sample, const int& N)
{
    for (int i = 0; i < N; i++)
        sample[i] = double(rand()) / RAND_MAX;
}

void getCosExpDistributionSample(const double& nu, const double& mu, const double& lambda, double*& sample, const int& N)
{
    const double a = PI * tan(PI * nu / 2);
    const double K = sin(PI * nu) * (1 / PI + PI / (a * a)) + nu;
    const double C = (sin(PI * nu) / PI + nu) / K; // C := P
    double r1 = 0;
    double r2 = 0;
    double r3 = 0;
    double r4 = 0;
    double x1 = 0;
    double x2 = 0;
    for (int i = 0; i < N; i++)
    {
        do r1 = (double)rand() / RAND_MAX; while (r1 == 0. || r1 == 1.);
        if (r1 <= C)
        {
            do
            {
                do r2 = (double)rand() / RAND_MAX; while (r2 == 0. || r2 == 1.);
                do r3 = (double)rand() / RAND_MAX; while (r3 == 0. || r3 == 1.);
                x1 = (2 * r2 - 1) * nu;
            } while (r3 > pow(cos(PI * x1 / 2), 2));
            sample[i] = x1 * lambda + mu;
        }
        else
        {
            do r4 = (double)rand() / RAND_MAX; while (r4 == 0. || r4 == 1.);
            x2 = nu - (log(r4) / a);
            if (r1 < ((1 + C) / 2))
                sample[i] = x2 * lambda + mu;
            else
                sample[i] = -x2 * lambda + mu;
        }
    }
}

void getDistributionMixtureSample(const double& nu1, const double& mu1, const double& lambda1, \
    const double& nu2, const double& mu2, const double& lambda2, const double& p, double*& sample, const int& N)
{
    const double a1 = PI * tan(PI * nu1 / 2);
    const double K1 = sin(PI * nu1) * (1 / PI + PI / (a1 * a1)) + nu1;
    const double C1 = (sin(PI * nu1) / PI + nu1) / K1; // C := P
    const double a2 = PI * tan(PI * nu2 / 2);
    const double K2 = sin(PI * nu2) * (1 / PI + PI / (a2 * a2)) + nu2;
    const double C2 = (sin(PI * nu2) / PI + nu2) / K2; // C := P
    double a, K, C;
    double nu, mu, lambda;
    double r, r1, r2, r3, r4, x1, x2;
    for (int i = 0; i < N; i++)
    {
        do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
        if (r > p)
        {
            a = a1;
            K = K1;
            C = C1;
            nu = nu1;
            mu = mu1;
            lambda = lambda1;
        }
        else
        {
            a = a2;
            K = K2;
            C = C2;
            nu = nu2;
            mu = mu2;
            lambda = lambda2;
        }
        do r1 = (double)rand() / RAND_MAX; while (r1 == 0. || r1 == 1.);
        if (r1 <= C)
        {
            do
            {
                do r2 = (double)rand() / RAND_MAX; while (r2 == 0. || r2 == 1.);
                do r3 = (double)rand() / RAND_MAX; while (r3 == 0. || r3 == 1.);
                x1 = (2 * r2 - 1) * nu;
            } while (r3 > pow(cos(PI * x1 / 2), 2));
            sample[i] = x1 * lambda + mu;
        }
        else
        {
            do r4 = (double)rand() / RAND_MAX; while (r4 == 0. || r4 == 1.);
            x2 = nu - (log(r4) / a);
            if (r1 < ((1 + C) / 2))
                sample[i] = x2 * lambda + mu;
            else
                sample[i] = -x2 * lambda + mu;
        }
    }
}

void getEmpiricDistributionSample(double* frequencies, double* start_sample, double*& target_sample, const int& N, const int& k)
{
    double x_min = DBL_MAX;
    double x_max = DBL_MIN;
    for (int i = 0; i < N; i++)
    {
        if (x_min > start_sample[i])
            x_min = start_sample[i];
        if (x_max < start_sample[i])
            x_max = start_sample[i];
    }
    double h = (x_max - x_min) / k;
    double r;
    int interval_index=0;
    bool generated = false;
    for (int c = 0; c < N; c++)
    {
        generated = false;
        do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
        for (int i = 0; i < k; i++)
        {
            r -= frequencies[i] / N;
            if (r <= 0)
            {
                interval_index = i;
                //target_sample[c] = i;
                break;
            }
        }
        do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
        target_sample[c] = (x_min + interval_index * h) + h * r;
    }
}

void writeSampleToFile(const char* file_name, const double* sample, const int& N)
{
    std::ofstream output_file;
    output_file.open(file_name);
    //output_file << std::setprecision(3);
    for (int i = 0; i < N; i++)
    {
        output_file << sample[i] << ' ';
    }
    output_file.close();
}

void readParameters(double& nu, double& mu, double& lambda, double& nu1, double& mu1, \
    double& lambda1, double& nu2, double& mu2, double& lambda2, double& p, int& N, double& x) {
    std::ifstream input_file;
    input_file.open("params.txt");
    if (!input_file.is_open())
    {
        std::cout << "Could not open the params file, exiting..";
        exit(0);
    }
    input_file >> nu >> mu >> lambda >> N >> x;
    if (nu <= 0 || nu > 1 || N < 0 || lambda <=0)
    {
        std::cout << "Incorrect parameters, exiting..";
        exit(0);
    }
    input_file >> nu1 >> mu1 >> lambda1 >> p;
    if (nu1 <= 0 || nu1 > 1 || p <= 0 || p >= 1 || lambda1 <= 0)
    {
        std::cout << "Incorrect parameters, exiting..";
        exit(0);
    }
    input_file >> nu2 >> mu2 >> lambda2;
    if (nu2 <= 0 || nu2 > 1 || lambda2 <= 0)
    {
        std::cout << "Incorrect parameters, exiting..";
        exit(0);
    }
    input_file.close();
}
