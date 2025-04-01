#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <cmath>
#include <chrono>
#include <string>

const double PI = M_PI;


class Primary
{
private:
	double shift;
	double scale;
	double form;
	double a;
	double K;
	double C; // C := P
public:
	Primary(double shift0 = 0, double scale0 = 1, double form0 = .5);

	Primary(std::string input_file_name);

	void setShift(double newShift);
	double getShift() const;

	void setScale(double newScale);
	double getScale() const;

	void setForm(double newForm);
	double getForm() const;

	double getA() const;
	double getK() const;
	double getC() const;

	double randNum() const;
	//M, D, A, E
	double* getCharacteristics() const;

	double getDensityFunctionOfX(double x) const;

	void save(std::string output_file_name) const;

	void load(std::string input_file_name);
};