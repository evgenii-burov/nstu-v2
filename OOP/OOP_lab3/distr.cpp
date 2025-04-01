#include "distr.h"

namespace Distr
{
	//Primary

	Primary::Primary(double shift0, double scale0, double form0) :
		shift(shift0),
		scale(scale0 > 0 ? scale0 : throw "Incorrect parameter: scale"),
		form(form0 > 0 && form0 < 1 ? form0 : throw "Incorrect parameter: form")
	{
		a = PI * tan(PI * form / 2);
		K = sin(PI * form) * (1 / PI + PI / (a * a)) + form;
		C = (sin(PI * form) / PI + form) / K;
	}

	Primary::Primary(std::string input_file_name)
	{
		std::ifstream input_file;
		input_file.open(input_file_name);
		if (!input_file.is_open())
		{
			throw(std::pair<std::string, std::string>("Unable to open file: ", input_file_name));
		}
		double shift0, scale0, form0;
		input_file >> shift0 >> scale0 >> form0;
		input_file.close();
		shift = shift0;
		scale = scale0 > 0 ? scale0 : throw "Incorrect parameter: scale";
		form = form0 > 0 && form0 < 1 ? form0 : throw "Incorrect parameter: form";
		a = PI * tan(PI * form / 2);
		K = sin(PI * form) * (1 / PI + PI / (a * a)) + form;
		C = (sin(PI * form) / PI + form) / K;
	}

    double Primary::randNum() const
    {
        double r1, r2, r3, r4, x1, x2;
        do r1 = (double)rand() / RAND_MAX; while (r1 == 0. || r1 == 1.);
        if (r1 <= C)
        {
            do
            {
                do r2 = (double)rand() / RAND_MAX; while (r2 == 0. || r2 == 1.);
                do r3 = (double)rand() / RAND_MAX; while (r3 == 0. || r3 == 1.);
                x1 = (2 * r2 - 1) * form;
            } while (r3 > pow(cos(PI * x1 / 2), 2));
            return x1 * scale + shift;
        }
        else
        {
            do r4 = (double)rand() / RAND_MAX; while (r4 == 0. || r4 == 1.);
            x2 = form - (log(r4) / a);
            if (r1 < ((1 + C) / 2))
                return x2 * scale + shift;
            else
                return -x2 * scale + shift;
        }
    }

    double* Primary::getCharacteristics() const
    {
        double M, D, A, E;
        M = shift; //shift scale transformation
        D = 2 * (pow(PI * form, 3) / 6 + (pow(PI * form, 2) / 2 - 1) * sin(PI * form) + PI * form * cos(PI * form)) \
            / (pow(PI, 3) * K) + 2 * pow(cos(PI * form / 2), 2) * (2 / (a * a) + 2 * form / a + form * form) / (a * K);
        D *= pow(scale, 2); //shift scale transformation
        A = 0;
        E = 4 * ((pow(PI * form, 5) / 20) + (sin(PI * form) * (pow(PI * form, 4) / 4 - 3 * pow(PI * form, 2) + 6)) + (cos(PI * form) * (pow(PI * form, 3) - 6 * PI * form))) / \
            (pow(D, 2) * pow(PI, 5) * K) + (2 * pow(cos(PI * form / 2), 2) / (pow(D, 2) * K)) * \
            (24 / pow(a, 5) + 24 * form / pow(a, 4) + 12 * pow(form, 2) / pow(a, 3) + 4 * pow(form, 3) / pow(a, 2) + pow(form, 4) / pow(a, 1)) - 3;
        E = (E + 3) * pow(scale, 4) - 3; //shift scale transformation
        double* characteristics=new double[4];
		characteristics[0] = M;
		characteristics[1] = D;
		characteristics[2] = A;
		characteristics[3] = E;
        return characteristics;
    }

    double Primary::getDensityFunctionOfX(double x) const
    {
        x = (x - shift) / scale;
        double f_of_x;
        if (abs(x) <= form)
        {
            f_of_x = pow(cos(PI * x / 2), 2);
        }
        else
        {
            f_of_x = pow(cos(PI * form / 2), 2) * exp(-1 * a * (abs(x) - form));
        }
        return f_of_x / (K * scale);
    }

    void Primary::save(std::string output_file_name) const
    {
        std::ofstream output_file;
        output_file.open(output_file_name);
        if (!output_file.is_open())
        {
            throw "Unable to open primary save file";
        }
        output_file << shift << '\t' << scale << '\t' << form << '\n';
        output_file.close();
    }

    void Primary::load(std::string input_file_name)
    {
        std::ifstream input_file;
        input_file.open(input_file_name);
        if (!input_file.is_open())
        {
            throw "Unable to open primary load file";
        }
        double shift0, scale0, form0;
        input_file >> shift0 >> scale0 >> form0;
        input_file.close();
        shift = shift0;
        scale = scale0 > 0 ? scale0 : throw "Incorrect parameter : scale";
        form = form0 > 0 && form0 < 1 ? form0 : throw "Incorrect parameter: form";
    }

	void Primary::setShift(double newShift) { shift = newShift; }
	double Primary::getShift() const { return shift; }

	void Primary::setScale(double newScale) { scale = newScale > 0 ? newScale : throw "Incorrect parameter: scale"; }
	double Primary::getScale() const { return scale; }

	void Primary::setForm(double newForm)
	{
		form = newForm > 0 && newForm < 1 ? newForm : throw "Incorrect parameter: form";
		a = PI * tan(PI * form / 2);
		K = sin(PI * form) * (1 / PI + PI / (a * a)) + form;
		C = (sin(PI * form) / PI + form) / K;
	}
	double Primary::getForm() const { return form; }

	double Primary::getA() const { return a; }
	double Primary::getK() const { return K; }
	double Primary::getC() const { return C; }


	//Empiric

	Empiric::Empiric(std::string input_file_name)
	{
		std::ifstream input_file;
		input_file.open(input_file_name);
		if (!input_file.is_open())
		{
			throw "Unable to open empiric constructor file";
		}
		int n0, k0;
		input_file >> n0 >> k0;
		n = n0 > 1 ? n0 : throw "Incorrect parameter : n";
		k = k0 > 1 ? k0 : int(log2(n)) + 1;
		data = new double[n];
		data_min = DBL_MAX;
		data_max = DBL_MIN;
		for (int i = 0; i < n; i++)
		{
			input_file >> data[i];
			if (data[i] < data_min)
				data_min = data[i];
			if (data[i] > data_max)
				data_max = data[i];
		}
		freq = new double[k];
		for (int i = 0; i < k; i++)
		{
			input_file >> freq[i];
		}
		input_file.close();
	}

	Empiric::Empiric(int n0, IDistribution& d, int k0) :
		n(n0 > 1 ? n0 : throw "Incorrect parameter: n"),
		k(k0 > 1 ? k0 : int(log2(n)) + 1),
		data(new double[n])
	{
		for (int i = 0; i < n; i++)
		{
			data[i] = d.randNum();
		}
		data_min = DBL_MAX;
		data_max = DBL_MIN;
		for (int i = 0; i < n; i++)
		{
			if (data[i] > data_max)
				data_max = data[i];
			if (data[i] < data_min)
				data_min = data[i];
		}
		freq = getDataFrequencies();
	}

	Empiric::~Empiric()
	{
		delete[] data;
		delete[] freq;
	}

	Empiric::Empiric(const Empiric& emp) :
		n(emp.n),
		k(emp.k),
		data_min(emp.data_min),
		data_max(emp.data_max)
	{
		delete[] data;
		if (n)
		{
			data = new double[n];
			for (int i = 0; i < n; i++)
			{
				data[i] = emp.data[i];
			}
		}
		else
			data = nullptr;
		delete[] freq;
		if (k)
		{
			freq = new double[k];
			for (int i = 0; i < k; i++)
			{
				freq[i] = emp.freq[i];
			}
		}
		else
			freq = nullptr;
	}

	Empiric& Empiric::operator=(const Empiric& emp)
	{
		if (this == &emp) return *this;

		if (n != emp.n) {
			delete[] data;
			n = emp.n;
			data = new double[n];
			for (int i = 0; i < n; i++)
			{
				data[i] = emp.data[i];
			}
		}
		if (k != emp.k) {
			delete[] freq;
			k = emp.k;
			freq = new double[k];
			for (int i = 0; i < k; i++)
			{
				freq[i] = emp.freq[i];
			}
		}
		return *this;
	}

	double* Empiric::getDataFrequencies()
	{
		freq = new double[k];
		double R = data_max - data_min;
		double h = R / k;
		int interval_index = 0;
		for (int i = 0; i < k; i++)
		{
			freq[i] = 0;
		}
		for (int i = 0; i < n; i++)
		{
			interval_index = int((data[i] - data_min) / h); //(data[i] - x_min)/h = 0..k
			interval_index -= int(((data[i] - data_min) / h) / k);
			freq[interval_index]++;
		}
		for (int i = 0; i < k; i++)
		{
			freq[i] /= n;
		}
		return freq;
	}

	double Empiric::randNum() const
	{
		double x_min = data_min;
		double x_max = data_max;
		double h = (x_max - x_min) / k;
		double r;
		int interval_index = 0;
		do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
		for (int i = 0; i < k; i++)
		{
			r -= freq[i];
			if (r <= 0)
			{
				interval_index = i;
				//target_sample[c] = i;
				break;
			}
		}
		do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
		return (x_min + interval_index * h) + h * r;
	}

	double* Empiric::getCharacteristics() const
	{
		double d = 0;
		double m = 0;
		double a = 0;
		double e = 0;
		for (int i = 0; i < n; i++)
		{
			m += data[i];
		}
		m = m / n;
		for (int i = 0; i < n; i++)
		{
			d += pow((data[i] - m), 2);
		}
		d = d / n;
		for (int i = 0; i < n; i++)
		{
			a += pow((data[i] - m), 3);
		}
		a = a / (n * pow(d, 1.5));
		for (int i = 0; i < n; i++)
		{
			e += pow((data[i] - m), 4);
		}
		e = e / (n * pow(d, 2)) - 3;
		double* characteristics = new double[4];
		characteristics[0] = m;
		characteristics[1] = d;
		characteristics[2] = a;
		characteristics[3] = e;
		return characteristics;
	}

	double Empiric::getDensityFunctionOfX(double x) const
	{
		double x_min = data_min;
		double x_max = data_max;

		double R = x_max - x_min;
		double h = R / k;
		int interval_index = 0;

		interval_index = int((x - x_min) / h);
		interval_index -= int(((x - x_min) / h) / k); // prevents freq[k] if x == x_max
		return freq[interval_index];
	}

	void Empiric::save(std::string output_file_name) const
	{
		std::ofstream output_file;
		output_file.open(output_file_name);
		if (!output_file.is_open())
		{
			throw "Unable to open empiric save file";
		}
		output_file << n << "\t" << k << "\n";
		for (int i = 0; i < n; i++)
		{
			output_file << data[i] << " ";
		}
		output_file << "\n";
		for (int i = 0; i < k; i++)
		{
			output_file << freq[i] << " ";
		}
		output_file << "\n";
		output_file.close();
	}

	void Empiric::load(std::string input_file_name)
	{
		std::ifstream input_file;
		input_file.open(input_file_name);
		if (!input_file.is_open())
		{
			throw "Unable to open empiric load file";
		}
		int n0, k0;
		input_file >> n0 >> k0;
		n = n0 > 1 ? n0 : throw "Incorrect parameter : n";
		k = k0 > 1 ? k0 : int(log2(n)) + 1;
		data_min = DBL_MAX;
		data_max = DBL_MIN;
		delete[] data;
		if (n)
		{
			data = new double[n];
			for (int i = 0; i < n; i++)
			{
				input_file >> data[i];
				if (data[i] < data_min)
					data_min = data[i];
				if (data[i] > data_max)
					data_max = data[i];
			}
		}
		else
			data = nullptr;
		delete[] freq;
		if (k)
		{
			freq = new double[k];
			for (int i = 0; i < k; i++)
			{
				input_file >> freq[i];
			}
		}
		else
			freq = nullptr;
		input_file.close();
	}

}