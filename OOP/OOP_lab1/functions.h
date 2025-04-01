#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <locale.h>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <cmath>
#include <chrono>

const double PI = M_PI;
//Read parameters from a file and check if they lie within their respective boundaries 
void readParameters(double& nu, double& mu, double& lambda, double& nu1, double& mu1, \
	double& lambda1, double& nu2, double& mu2, double& lambda2, double& p, int& N, double& x);
//Get the frequencies at which elements of the sample appear
void getSampleFrequencies(double*& sample, double*& frequencies, const int& N, const int& k);
//Calculate empiric characteristics of a sample and either return one of them or print all of them in console
double calculateEmpiricSampleCharacteristic(double*& sample, const int& N, const char& value_desired);
//Calculate a chosen theoretic characteristic of the standart distribution
double calculateTheoreticStandartCharacteristic(const double& nu, const double& mu, const double& lambda,\
	const char& value_desired, double x=0);
//Calculate a chosen theoretic characteristic of the mixed distribution
double calculateTheoreticMixCharacteristic(const double& nu1, const double& mu1, const double& lambda1, \
	const double& nu2, const double& mu2, const double& lambda2, const double& p, \
	const char& value_desired, double x=0);

void UniformDistribution(double*& sample, const int& N);

void getCosExpDistributionSample(const double& nu, const double& mu, const double& lambda, double*& sample, const int& N);

void getDistributionMixtureSample(const double& nu1, const double& mu1, const double& lambda1, \
	const double& nu2, const double& mu2, const double& lambda2, const double& p, double*& sample, const int& N);

void getEmpiricDistributionSample(double* frequencies, double* start_sample, double*& target_sample, const int& N, const int& k);

void writeSampleToFile(const char* file_name, const double* sample, const int& N);

