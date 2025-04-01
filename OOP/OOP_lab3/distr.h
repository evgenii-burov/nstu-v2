#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <iomanip>

#define interface struct

namespace Distr
{
    const double PI = M_PI;

	__interface IDistribution
	{
		double randNum() const;

		double* getCharacteristics() const;

		double getDensityFunctionOfX(double x) const;
	};

	__interface IPersistent
	{
		void save(std::string output_file_name) const;

		void load(std::string input_file_name);
	};

	class Primary : public IDistribution, public IPersistent
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

		double randNum() const override;
		//M, D, A, E
		double* getCharacteristics() const override;

		double getDensityFunctionOfX(double x) const override;

		void save(std::string output_file_name) const override;

		void load(std::string input_file_name) override;
	};

	class Empiric : public IDistribution, public IPersistent
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

		Empiric(int n0, IDistribution& d, int k0 = 1);

		~Empiric();

		Empiric(const Empiric& emp);
		Empiric& operator=(const Empiric& emp);

		double randNum() const override;
		//M, D, A, E
		double* getCharacteristics() const override;

		double getDensityFunctionOfX(double x) const override;

		void save(std::string output_file_name) const override;

		void load(std::string input_file_name) override;
	};

	template<class Distribution1, class Distribution2>
	class Mixture : public IDistribution, public IPersistent
	{
	private:
		double p;
		Distribution1 d1;
		Distribution2 d2;
	public:
		Mixture(Distribution1& d10, Distribution2& d20, double p0) :
			p(p0 > 0 && p0 < 1 ? p0 : throw "Incorrect parameter: p"),
			d1(d10),
			d2(d20)
		{}

		Distribution1& component1() { return d1; }

		Distribution2& component2() { return d2; }

		void setP(double pNew)
		{
			p = pNew > 0 && pNew < 1 ? pNew : throw(std::pair<std::string, double>("Incorrect parameter: ,", pNew));
		}

		double getP() const { return p; }

		double randNum() const override
		{
			double r;
			do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
			if (r < p)
				return d1.randNum();
			else
				return d2.randNum();
		}

		double* getCharacteristics() const override
		{
			double M, D, A, E;
			double m1, m2, sigma_sqr1, sigma_sqr2, e1, e2;
			double a_first_part, a_second_part, e_first_part, e_second_part;
			sigma_sqr1 = d1.getCharacteristics()[1];
			sigma_sqr2 = d2.getCharacteristics()[1];
			e1 = d2.getCharacteristics()[3];
			e2 = d2.getCharacteristics()[3];

			m1 = d1.getCharacteristics()[0];
			m2 = d2.getCharacteristics()[0];

			M = (1 - p) * m1 + p * m2;
			D = (1 - p) * (pow(m1, 2) + sigma_sqr1) + p * (pow(m2, 2) + sigma_sqr2) - pow(M, 2);

			a_first_part = pow((m1 - M), 3) + 3 * (m1 - M) * sigma_sqr1;
			a_second_part = pow((m2 - M), 3) + 3 * (m2 - M) * sigma_sqr2;
			A = ((1 - p) * a_first_part + p * a_second_part) / pow(D, 1.5);

			e_first_part = pow((m1 - M), 4) + 6 * pow((m1 - M), 2) * sigma_sqr1 + pow(sigma_sqr1, 2) * (e1 + 3);
			e_second_part = pow((m2 - M), 4) + 6 * pow((m2 - M), 2) * sigma_sqr2 + pow(sigma_sqr2, 2) * (e2 + 3);
			E = (((1 - p) * e_first_part + p * e_second_part) / pow(D, 2)) - 3;
			double* characteristics = new double[4];
			characteristics[0] = M;
			characteristics[1] = D;
			characteristics[2] = A;
			characteristics[3] = E;
			return characteristics;
		}

		double getDensityFunctionOfX(double x) const override
		{
			return ((1 - p) * d1.getDensityFunctionOfX(x) + p * d2.getDensityFunctionOfX(x));
		}

		void save(std::string output_file_name) const override
		{
			d1.save(std::string("1") + output_file_name);
			d2.save(std::string("2") + output_file_name);

			std::ofstream output_stream(std::string("p_") + output_file_name);
			if (!output_stream)
				throw std::string("Couldn't open file: " + std::string("p_") + output_file_name);
			output_stream << p;
			output_stream.close();
		}

		void load(std::string input_file_name)
		{
			d1.load(std::string("1") + input_file_name);
			d2.load(std::string("2") + input_file_name);

			std::ifstream input_stream(std::string("p_") + input_file_name);
			if (!input_stream)
				throw std::string("Couldn't open file: " + std::string("p_") + input_file_name);
			double p0;
			input_stream >> p0;
			p = p0 > 0 && p0 < 1 ? p0 : throw("Incorrect parameter: p");
			input_stream.close();
		}

	};
}

using namespace Distr;