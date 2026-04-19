#pragma once
#include "functions.h"

class FunctionMinimization {
protected:
    virtual string name() = 0;

    int func_evaluations = 0;
    int iterations_count = 0;

    Function& func;

    double eps_f;
    double eps_x;

    double goldenSection(Function& func, const Point& start, const Point& direction) {
        auto f_lambda = [&](double l) {
            Point p = start + direction * l;
            return func(p);
            };
        double a = 0, delta = 1, h = delta;
        double b = 1;

        while (f_lambda(b) < f_lambda(a)){
            func_evaluations += 2;
            b = a + h;
            h *= 2;
        }

        //std::cout << "a:" << a << " b:" << b;

        double phi = (1.0 + sqrt(5.0)) / 2.0;
        double x1 = b - (b - a) / phi;
        double x2 = a + (b - a) / phi;

        double y1 = f_lambda(x1);
        double y2 = f_lambda(x2);
        func_evaluations += 2;

        while (abs(b - a) > eps_x) {
            if (y1 < y2) {
                b = x2;
                x2 = x1;
                y2 = y1;
                x1 = b - (b - a) / phi;
                y1 = f_lambda(x1);
            }
            else {
                a = x1;
                x1 = x2;
                y1 = y2;
                x2 = a + (b - a) / phi;
                y2 = f_lambda(x2);
            }
            func_evaluations++;
        }

        return (a + b) / 2.0;
    }

public:
    FunctionMinimization(Function& function0, double eps_f0, double eps_x0) : func(function0), eps_f(eps_f0), eps_x(eps_x0)
    {};

    virtual void minimize(Point start, bool verbose = false) = 0;
};

class HookJeeves : public FunctionMinimization {
private:
    string name() override {
        return "Hook-Jeeves";
    }

public:
    using FunctionMinimization::FunctionMinimization;

    void minimize(Point start, bool verbose = false) override {
        cout << scientific;
        cout << "Minimization method: " << name() << endl;
        cout << "Function: " << func.name() << endl;

        Point x_curr = start;
        Point x_prev = x_curr;
        double f_curr = func(x_curr);
        func_evaluations = 1;
        double f_prev = f_curr;

        if (verbose) {
            cout << setw(14) << "x"
                << setw(14) << "y"
                << setw(14) << "f(x,y)"
                << setw(14) << "d1"
                << setw(14) << "d2"
                << setw(14) << "lambda"
                << setw(14) << "dx"
                << setw(14) << "dy"
                << setw(14) << "df" << "\n";
        }

        do {
            iterations_count++;

            bool unsuccesfull_step_x = false;
            bool unsuccesfull_step_y = false;
            double step = 1.0;
            double alpha = 0.5;

            x_prev = x_curr;
            f_prev = f_curr;

            //Probing search
            do {
                // Probing on x
                Point x_pos = x_curr + Point(step, 0);
                Point x_neg = x_curr + Point(-step, 0);

                double f_x_pos = func(x_pos);
                double f_x_neg = func(x_neg);
                func_evaluations += 2;

                if (f_x_pos < f_curr) {
                    if (f_x_pos < f_x_neg) {
                        f_curr = f_x_pos;
                        x_curr = x_pos;
                    }
                    else {
                        f_curr = f_x_neg;
                        x_curr = x_neg;
                    }
                }
                else {
                    if (f_x_neg < f_curr) {
                        f_curr = f_x_neg;
                        x_curr = x_neg;
                    }
                    else {
                        unsuccesfull_step_x = true;
                    }
                }
                // Probing on y
                Point y_pos = x_curr + Point(0, step);
                Point y_neg = x_curr + Point(0, -step);

                double f_y_pos = func(y_pos);
                double f_y_neg = func(y_neg);
                func_evaluations += 2;

                if (f_y_pos < f_curr) {
                    if (f_y_pos < f_y_neg) {
                        f_curr = f_y_pos;
                        x_curr = y_pos;
                    }
                    else {
                        f_curr = f_y_neg;
                        x_curr = y_neg;
                    }
                }
                else {
                    if (f_y_neg < f_curr) {
                        f_curr = f_y_neg;
                        x_curr = y_neg;
                    }
                    else {
                        unsuccesfull_step_y = true;
                    }
                }

                step *= alpha;

            } while (unsuccesfull_step_x && unsuccesfull_step_y && step > eps_x);

            //Pattern search
            Point direction = x_curr - x_prev;
            direction = direction * (1 / (sqrt(direction.x * direction.x + direction.y * direction.y)));

            double lambda = goldenSection(func, x_curr, direction);

            x_curr = x_curr + direction * lambda;

            if (verbose){
                cout << setw(14) << x_curr.x
                    << setw(14) << x_curr.y
                    << setw(14) << func(x_curr)
                    << setw(14) << direction.x
                    << setw(14) << direction.y
                    << setw(14) << lambda
                    << setw(14) << abs(x_curr.x - x_prev.x)
                    << setw(14) << abs(x_curr.y - x_prev.y)
                    << setw(14) << abs(f_curr - f_prev) << "\n";
            }

            
            if (abs(f_curr - f_prev) < eps_f)
                cout << "FFFF";
            if (sqrt(pow(x_curr.x - x_prev.x, 2) + pow(x_curr.y - x_prev.y, 2)) < eps_x)
                cout << "XXXX";

        } while (abs(f_curr - f_prev) > eps_f && sqrt(pow(x_curr.x - x_prev.x, 2) + pow(x_curr.y - x_prev.y, 2)) > eps_x && iterations_count < 1000);

        cout << "Minimization results:" << endl;
        cout << "(x0,y0)\t\teps_f\t\teps_x\t\titer\t\tf_eval\t\t(x,y)\t\tf\n";
        cout << start << "\t\t" << eps_f << "\t\t" << eps_x << "\t\t" << iterations_count << "\t\t";
        cout << func_evaluations << "\t\t" << x_curr << "\t\t" << func(x_curr) << endl;

    }
};

class GradientDescent : public FunctionMinimization {
private:
    string name() override {
        return "Gradient descent";
    }

public:
    using FunctionMinimization::FunctionMinimization;

    void minimize(Point start, bool verbose = false) override {
        cout << scientific;
        cout << "Minimization method: " << name() << endl;
        cout << "Function: " << func.name() << endl;

        if (verbose) {
            cout << setw(14) << "x"
                << setw(14) << "y"
                << setw(14) << "f(x,y)"
                << setw(14) << "d1"
                << setw(14) << "d2"
                << setw(14) << "lambda"
                << setw(14) << "dx"
                << setw(14) << "dy"
                << setw(14) << "df"
                << setw(14) << "grad.x"
                << setw(14) << "grad.y" << "\n";
        }

        Point x_curr = start;
        Point x_prev = x_curr;
        double f_curr = func(x_curr);
        func_evaluations = 1;
        double f_prev = f_curr;

        do {
            x_prev = x_curr;
            f_prev = f_curr;
            Point direction = func.grad(x_curr) * -1;
            double gradNorm = sqrt(direction.x * direction.x + direction.y * direction.y);
            direction = direction * (1 / gradNorm);
            double lambda = goldenSection(func, x_curr, direction);
            
            x_curr = x_curr + direction * lambda;
            f_curr = func(x_curr);
            func_evaluations++;
            if (verbose) {
                cout << setw(14) << x_curr.x
                    << setw(14) << x_curr.y
                    << setw(14) << f_curr
                    << setw(14) << direction.x
                    << setw(14) << direction.y
                    << setw(14) << lambda
                    << setw(14) << abs(x_curr.x - x_prev.x)
                    << setw(14) << abs(x_curr.y - x_prev.y)
                    << setw(14) << abs(f_curr - f_prev)
                    << setw(14) << func.grad(x_curr).x
                    << setw(14) << func.grad(x_curr).y << "\n";
            }
        } while (abs(f_curr - f_prev) > eps_f && sqrt(pow(x_curr.x - x_prev.x, 2) + pow(x_curr.y - x_prev.y, 2)) > eps_x && iterations_count < 1000);
        
        cout << "Minimization results:" << endl;
        cout << "(x0,y0)\t\teps_f\t\teps_x\t\titer\t\tf_eval\t\t(x,y)\t\tf\n";
        cout << start << "\t\t" << eps_f << "\t\t" << eps_x << "\t\t" << iterations_count << "\t\t";
        cout << func_evaluations << "\t\t" << x_curr << "\t\t" << func(x_curr) << endl;
    }
};
