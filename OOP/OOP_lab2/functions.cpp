#include "primary.h"
#include "mixture.h"
#include "header.h"
#include "empiric.h"

double calculateEmpiricSampleCharacteristic(double*& sample, const int& n, const char& value_desired)
{
    double d = 0;
    double m = 0;
    double a = 0;
    double e = 0;
    for (int i = 0; i < n; i++)
    {
        m += sample[i];
    }
    m = m / n;
    for (int i = 0; i < n; i++)
    {
        d += pow((sample[i] - m), 2);
    }
    d = d / n;
    for (int i = 0; i < n; i++)
    {
        a += pow((sample[i] - m), 3);
    }
    a = a / (n * pow(d, 1.5));
    for (int i = 0; i < n; i++)
    {
        e += pow((sample[i] - m), 4);
    }
    e = e / (n * pow(d, 2)) - 3;
    switch (value_desired)
    {
    default:
        std::cout << "Empiric values:\nm:\t" << m << "\nd:\t" << d << "\na:\t" << a << "\ne:\t" << e << "\n\n";
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

void setParametersPrimary(Primary& primary, double shift0, double scale0, double form0)
{
    std::cout << "Setting parameters:\n" << "shift: " << shift0 << '\n' << "scale: " << scale0 << '\n' << "form: " << form0 << "\n";
    primary.setShift(shift0);
    primary.setScale(scale0);
    primary.setForm(form0);
    std::cout << "Parameters set.\n\n";
}

void getParametersPrimary(Primary& primary)
{
    std::cout << "Getting parameters:\n" << "shift: " << primary.getShift() << '\n' << "scale: " << primary.getScale() << '\n' << "form: " << primary.getForm() << "\n\n";
}

void getSampleFrequencies(double* sample, double* frequencies, int n, int k)
{
    double x_min = DBL_MAX;
    double x_max = DBL_MIN;
    for (int i = 0; i < n; i++)
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
    for (int i = 0; i < n; i++)
    {
        interval_index = int((sample[i] - x_min) / h); //(sample[i] - x_min)/h = 0..k
        interval_index -= int(((sample[i] - x_min) / h) / k);
        frequencies[interval_index]++;
    }
}

void writeSampleToFile(std::string file_name, double* sample, int n)
{
    std::ofstream output_file;
    output_file.open(file_name);
    for (int i = 0; i < n; i++)
    {
        output_file << sample[i] << ' ';
    }
    output_file.close();
}

void testPrimary()
{
    try
    {
        Primary primary1(0, -2, .6);
        getParametersPrimary(primary1);
        Primary primary2("primary_init.txt");
        getParametersPrimary(primary2);
        Primary primary3;
        primary3.load("primary_save.txt");
        getParametersPrimary(primary3);
        
        setParametersPrimary(primary1, 0, 1, .2);
        getParametersPrimary(primary1);

        int n = 100;
        double* primary_sample = new double[n];
        int k = int(log2(n)) + 1;
        double* frequencies = new double[k];
        for (int i = 0; i < n; i++)
        {
            primary_sample[i] = primary1.randNum();
        }
        getSampleFrequencies(primary_sample, frequencies, n, k);
        
        std::cout << std::setprecision(3);
        calculateEmpiricSampleCharacteristic(primary_sample, n, '0');
        std::cout << "Theoretical values:\n";
        char values[4] = { 'M', 'D', 'A', 'E' };
        for (int i = 0; i < 4; i++)
        {
            std::cout << values[i] << '\t' << primary1.getValue(values[i]) << "\n";
        }
        std::cout << "\n\n";

        writeSampleToFile("primary_frequencies.txt", frequencies, k);
        delete[] primary_sample;
        delete[] frequencies;

        std::cout <<"Density function of 0: " << primary1.getDensityFunctionOfX(0);
        primary1.save("primary_save.txt");
    }
    catch (std::pair<std::string, double> error)
    {
        std::cerr << error.first << error.second;
    }
    catch (std::pair<std::string, std::string> error)
    {
        std::cerr << error.first << error.second;
    }
    catch (std::pair<std::string, char> error)
    {
        std::cerr << error.first << error.second;
    }
}

void testMixture()
{
    try
    {
        Primary primary1(-1, 1, .5);
        getParametersPrimary(primary1);
        Primary primary2("primary_init.txt");
        getParametersPrimary(primary2);
        Mixture mix1 = Mixture(primary1, primary2, .5);

        int n = 100;
        double* mixture_sample = new double[n];
        int k = int(log2(n)) + 1;
        double* mixture_frequencies = new double[k];
        for (int i = 0; i < n; i++)
        {
            mixture_sample[i] = mix1.randNum();
        }
        getSampleFrequencies(mixture_sample, mixture_frequencies, n, k);

        std::cout << std::setprecision(3);
        calculateEmpiricSampleCharacteristic(mixture_sample, n, '0');
        std::cout << "Theoretical values:\n";
        char values[4] = { 'M', 'D', 'A', 'E' };
        for (int i = 0; i < 4; i++)
        {
            std::cout << values[i] << '\t' << primary1.getValue(values[i]) << "\n";
        }
        std::cout << "\n\n";

        writeSampleToFile("mixture_frequencies.txt", mixture_frequencies, k);
        writeSampleToFile("mixture_frequencies.txt", mixture_sample, n);
        delete[] mixture_sample;
        delete[] mixture_frequencies;

        std::cout << "Density function of 0: " << mix1.getDensityFunctionOfX(0);
        primary1.save("primary_save.txt");
    }
    catch (std::pair<std::string, double> error)
    {
        std::cerr << error.first << error.second;
    }
    catch (std::pair<std::string, std::string> error)
    {
        std::cerr << error.first << error.second;
    }
    catch (std::pair<std::string, char> error)
    {
        std::cerr << error.first << error.second;
    }
}

void testEmpiric()
{
    Primary d1 = Primary(0, 1, .8);
    Empiric emp1 = Empiric(100, d1);
    emp1 = emp1;
    Primary d2 = Primary(0, 1, .2);
    Mixture mix1 = Mixture(d1, d2, .5);
    srand(10);
    Empiric emp2 = Empiric(1000000, mix1);
    //emp2 = emp1;
    //Empiric emp3 = Empiric(150, emp3);

    int n = 1000000;
    double* mixture_sample = new double[n];
    int k = int(log2(n)) + 1;
    double* mixture_frequencies = new double[k];
    srand(10);
    for (int i = 0; i < n; i++)
    {
        mixture_sample[i] = mix1.randNum();
    }
    getSampleFrequencies(mixture_sample, mixture_frequencies, n, k);

    calculateEmpiricSampleCharacteristic(mixture_sample, n, '0');
    std::cout << "Theoretical values:\n";
    char values[4] = { 'M', 'D', 'A', 'E' };
    for (int i = 0; i < 4; i++)
    {
        std::cout << values[i] << '\t' << emp2.getValue(values[i]) << "\n";
    }
    std::cout << "\n\n";
    std::cout << "Density function of 0: " << emp2.getDensityFunctionOfX(0);
    emp2.save("empiric_size.txt", "empiric_data.txt", "empiric_freq.txt");
    delete[] mixture_sample;
    delete[] mixture_frequencies;
}