#include "functions.h"
//#include "functions.cpp"

int main() 
{
    //declaring variables
    auto t1 = std::chrono::high_resolution_clock::now();
    //srand((unsigned)time(0));
    srand(12);
    double nu, mu, lambda, x;
    double nu1, mu1, lambda1, nu2, mu2, lambda2, p;
    int N;

    //reading parameters from a file and printing them in console
    readParameters(nu, mu, lambda, nu1, mu1, lambda1, nu2, mu2, lambda2, p, N, x);
    int k = int(log2(N)) + 1; // int(log2(N)) + 1
    std::cout << "Parameters:\n";
    std::cout <<"nu: " << nu << "\tmu: " << mu << "\tlambda: " << lambda << "\tN: "\
        << N << "\tk: " << k<< "\tp: " << p<< "\tx: " << x <<"\n";
    std::cout << "nu1: " << nu1 << "\tmu1: " << mu1 << "\tlambda1: " << lambda1 << "\n";
    std::cout << "nu2: " << nu2 << "\tmu2: " << mu2 << "\tlambda2: " << lambda2 << "\n\n";

    //modeling distributions
    double* standart_sample = new double[N];
    double* mixture_sample = new double[N];
    double* frequencies = new double[k];
    double* mixture_frequencies = new double[k];
    double* empiric_frequencies = new double[k];
    double* empiric_sample = new double[N];
    getCosExpDistributionSample(nu, mu, lambda, standart_sample, N);
    writeSampleToFile("standart_sample.txt", standart_sample, N);
    getSampleFrequencies(standart_sample, frequencies, N, k);
    writeSampleToFile("frequencies.txt", frequencies, k);
    getDistributionMixtureSample(nu1, mu1, lambda1, nu2, mu2, lambda2, p, mixture_sample, N);
    writeSampleToFile("mixture_sample.txt", mixture_sample, N);
    getSampleFrequencies(mixture_sample, mixture_frequencies, N, k);
    writeSampleToFile("mixture_frequencies.txt", mixture_frequencies, k);
    getEmpiricDistributionSample(frequencies, standart_sample, empiric_sample, N, k);
    writeSampleToFile("empiric_sample.txt", empiric_sample, N);
    getSampleFrequencies(empiric_sample, empiric_frequencies, N, k);
    writeSampleToFile("empiric_frequencies.txt", empiric_frequencies, k);

    std::cout << std::setprecision(3);
    //Standart distribution characteristics
    std::cout << "Standart distribution:\n\n";
    calculateEmpiricSampleCharacteristic(standart_sample, N, '0');
    std::cout << "Theoretical values:";
    char standart_distribution_characteristics[5] = {'M', 'D', 'A', 'E', 'F'};
    for (int i = 0; i < 5; i++)
    {
        std::cout << '\n' << standart_distribution_characteristics[i] << ":\t";
        std::cout << calculateTheoreticStandartCharacteristic(nu, mu, lambda, standart_distribution_characteristics[i], x) << '\t';
    }
    //Distribution mixture characteristics
    std::cout << "\n\nDistribution mixture:\n\n";
    calculateEmpiricSampleCharacteristic(mixture_sample, N, '0');
    std::cout << "Theoretical values:";
    char mix_distribution_characteristics[5] = { 'M', 'D', 'A', 'E', 'F' };
    for (int i = 0; i < 5; i++)
    {
        std::cout << '\n' << mix_distribution_characteristics[i] << ":\t";
        std::cout << calculateTheoreticMixCharacteristic(nu1, mu1, lambda1, nu2, mu2, lambda2, p, mix_distribution_characteristics[i], x) << '\t';
    }
    //Empiric distribution characteristics
    std::cout << "\n\nEmpiric distribution:\n";
    calculateEmpiricSampleCharacteristic(empiric_sample, N, '0');
    delete[] standart_sample;
    delete[] mixture_sample;
    delete[] empiric_sample;
    delete[] frequencies;
    delete[] mixture_frequencies;
    delete[] empiric_frequencies;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
    std::cout <<"Runtime (seconds): " << duration.count();
}
