#include "distr.h"

using namespace Distr;

//void testMixture()
//{
//    try
//    {
//        Primary primary1(-1, 1, .5);
//        getParametersPrimary(primary1);
//        Primary primary2("primary_init.txt");
//        getParametersPrimary(primary2);
//        Mixture mix1 = Mixture(primary1, primary2, .5);
//
//        int n = 100;
//        double* mixture_sample = new double[n];
//        int k = int(log2(n)) + 1;
//        double* mixture_frequencies = new double[k];
//        for (int i = 0; i < n; i++)
//        {
//            mixture_sample[i] = mix1.randNum();
//        }
//        getSampleFrequencies(mixture_sample, mixture_frequencies, n, k);
//
//        std::cout << std::setprecision(3);
//        calculateEmpiricSampleCharacteristic(mixture_sample, n, '0');
//        std::cout << "Theoretical values:\n";
//        char values[4] = { 'M', 'D', 'A', 'E' };
//        for (int i = 0; i < 4; i++)
//        {
//            std::cout << values[i] << '\t' << primary1.getValue(values[i]) << "\n";
//        }
//        std::cout << "\n\n";
//
//        writeSampleToFile("mixture_frequencies.txt", mixture_frequencies, k);
//        writeSampleToFile("mixture_frequencies.txt", mixture_sample, n);
//        delete[] mixture_sample;
//        delete[] mixture_frequencies;
//
//        std::cout << "Density function of 0: " << mix1.getDensityFunctionOfX(0);
//        primary1.save("primary_save.txt");
//    }
//    catch (std::pair<std::string, double> error)
//    {
//        std::cerr << error.first << error.second;
//    }
//    catch (std::pair<std::string, std::string> error)
//    {
//        std::cerr << error.first << error.second;
//    }
//    catch (std::pair<std::string, char> error)
//    {
//        std::cerr << error.first << error.second;
//    }
//}

int main()
{
    std::cout << std::setprecision(3);
    
    Primary pr = Primary(0, 1, .5);
    Empiric em_pr = Empiric(1000000, pr, 300);
    double* characteristics = pr.getCharacteristics();
    std::cout << "Primary distribution theoretical characteristics:\n";
    std::cout << "M\tD\tA\tE\n";
    for (int i = 0; i < 4; i++)
    {
        std::cout<<characteristics[i] << "\t";
    }
    std::cout << "\n";
    delete[] characteristics;

    characteristics = em_pr.getCharacteristics();
    std::cout << "Primary distribution empirical characteristics:\n";
    std::cout << "M\tD\tA\tE\n";
    for (int i = 0; i < 4; i++)
    {
        std::cout << characteristics[i] << "\t";
    }
    std::cout << "\n";
    delete[] characteristics;

    //Тестирование механизма позднего связывания
    IPersistent* persistent_array[2];
    persistent_array[0] = &pr;
    persistent_array[1] = &em_pr;
    persistent_array[0]->save("save_file_1.txt");
    persistent_array[1]->save("save_file_2.txt");

    Primary pr1 = Primary(-0.5, 1.2, .5);
    Primary pr2 = Primary(0, 1, .5);
    Primary pr3 = Primary(2, 2, .5);
    Primary pr4 = Primary(1.5, 1.2, .5);
    Mixture<Primary, Primary> mix1 = Mixture<Primary, Primary>(pr1, pr3, 0.5);
    Mixture<Mixture<Primary, Primary>, Primary> mix2 = Mixture<Mixture<Primary, Primary>, Primary>(mix1, pr2, .67);
    Mixture<Primary, Primary> mix3 = Mixture<Primary, Primary>(pr2, pr4, 0.5);
    Mixture<Mixture<Primary, Primary>, Mixture<Primary, Primary>> mix4 = Mixture<Mixture<Primary, Primary>, Mixture<Primary, Primary>>(mix1, mix3, .5);
    characteristics = mix2.getCharacteristics();
    std::cout << "Mixture distribution theoretical characteristics:\n";
    std::cout << "M\tD\tA\tE\n";
    for (int i = 0; i < 4; i++)
    {
        std::cout << characteristics[i] << "\t";
    }
    std::cout << "\n";
    delete[] characteristics;

    Empiric em_mix2 = Empiric(1000000, pr1);
    characteristics = em_mix2.getCharacteristics();
    std::cout << "Mixture distribution empirical characteristics:\n";
    std::cout << "M\tD\tA\tE\n";
    for (int i = 0; i < 4; i++)
    {
        std::cout << characteristics[i] << "\t";
    }
    std::cout << "\n";
    delete[] characteristics;
    em_mix2.save("mixture_sample.txt");

    Empiric em_em_pr = Empiric(100000, em_mix2);
    std::cout << "Empiric distribution empirical characteristics:\n";
    characteristics = em_em_pr.getCharacteristics();
    std::cout << "M\tD\tA\tE\n";
    for (int i = 0; i < 4; i++)
    {
        std::cout << characteristics[i] << "\t";
    }
    std::cout << "\n";
    delete[] characteristics;
    em_em_pr.save("mixture_sample.txt");


}
