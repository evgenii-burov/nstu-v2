#pragma once
#include "primary.h"

class Mixture
{
private:
	Primary d1, d2;
	double p;
public:
	Mixture(std::string input_file_name);
	Mixture(Primary prm10, Primary prm20, double p0);

	Primary& component1();
	Primary& component2();

	void setP(double pNew);
	double getP() const;

	double randNum() const;
	//M, D, A, E
	double* getCharacteristics() const;

	double getDensityFunctionOfX(double x) const;

	void save(std::string output_file_name) const;

	void load(std::string input_file_name);
};