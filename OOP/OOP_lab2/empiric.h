#pragma once
#include "primary.h"
#include "mixture.h"

class Empiric
{
private:
	//data size
	int n;
	//freq size
	int k;
	//data sample
	double* data;
	//data sample frequencies
	double* freq;
    double data_min;
    double data_max;

	double* getDataFrequencies();
public:
	Empiric(std::string input_file_name);

	Empiric(int n0, Primary& prim, int k0 = 1);
	Empiric(int n0, Mixture& mixt, int k0 = 1);
	Empiric(int n0, Empiric& emp, int k0 = 1);

	~Empiric();

	Empiric(const Empiric& emp);
	Empiric& operator=(const Empiric& emp);

	double randNum() const;
	//M, D, A, E
	double* getCharacteristics() const;

	double getDensityFunctionOfX(double x) const;

	void save(std::string output_file_name) const;

	void load(std::string input_file_name);
};
