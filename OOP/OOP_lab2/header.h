#include "primary.h"
#include "mixture.h"
#include "empiric.h"

double calculateEmpiricSampleCharacteristic(double*& sample, const int& N, const char& value_desired);

void setParametersPrimary(Primary& primary, double shift0, double scale0, double form0);

void getParametersPrimary(Primary& primary);

void getSampleFrequencies(double* sample, double* frequencies, int n, int k);

void writeSampleToFile(std::string file_name, double* sample, int n);

void testPrimary();

void testMixture();

void testEmpiric();