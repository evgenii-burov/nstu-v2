#include "primary.h"

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
    double characteristics[4]{M, D, A, E};
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
    input_file >> shift0 >>  scale0 >> form0;
    input_file.close();
    shift = shift0;
    scale = scale0 > 0 ? scale0 : throw "Incorrect parameter : scale";
    form = form0 > 0 && form0 < 1 ? form0 : throw "Incorrect parameter: form";
}