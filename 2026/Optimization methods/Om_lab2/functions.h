#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <functional>
#include <string>

using namespace std;

struct Point {
    double x, y;
    Point(double _x = 0, double _y = 0) : x(_x), y(_y) {}
    Point operator+(const Point& other) const { return Point(x + other.x, y + other.y); }
    Point operator-(const Point& other) const { return Point(x - other.x, y - other.y); }
    Point operator*(double scalar) const { return Point(x * scalar, y * scalar); }
    friend ostream& operator<<(ostream& out, const Point& p)
    {
        out << '(' << p.x << ", " << p.y << ')';
        return out;
    }
};

class Function {
public:
    virtual double operator()(const Point& p) = 0;
    virtual Point grad(const Point& p) { return Point(0, 0); }
    virtual string name() = 0;
    virtual ~Function() {}
};

class Rosenbrock : public Function {
public:
    double operator()(const Point& p) override {
        return 100.0 * pow(p.y - p.x * p.x, 2) + pow(1.0 - p.x, 2);
    }
    Point grad(const Point& p) override {
        double dfdx = -400.0 * p.x * (p.y - p.x * p.x) - 2.0 * (1.0 - p.x);
        double dfdy = 200.0 * (p.y - p.x * p.x);
        return Point(dfdx, dfdy);
    }
    string name() override { return "Rosenbrock"; }
};

class Variant4 : public Function {
    const double A1 = 2, A2 = 1;
    const double a1 = 1, a2 = 3;
    const double b1 = 2, b2 = 3;
    const double c1 = 2, c2 = 1;
    const double d1 = 1, d2 = 3;

    double calcF(const Point& p) {
        double term1 = 1.0 + pow((p.x - a1) / b1, 2) + pow((p.y - c1) / d1, 2);
        double term2 = 1.0 + pow((p.x - a2) / b2, 2) + pow((p.y - c2) / d2, 2);
        return (A1 / term1) + (A2 / term2);
    }
public:
    double operator()(const Point& p) override {
        return -calcF(p);
    }
    Point grad(const Point& p) override {
        double t1 = 1.0 + pow((p.x - a1) / b1, 2) + pow((p.y - c1) / d1, 2);
        double t2 = 1.0 + pow((p.x - a2) / b2, 2) + pow((p.y - c2) / d2, 2);

        double dfdx = -A1 * (2.0 * (p.x - a1) / (b1 * b1)) / (t1 * t1)
            - A2 * (2.0 * (p.x - a2) / (b2 * b2)) / (t2 * t2);

        double dfdy = -A1 * (2.0 * (p.y - c1) / (d1 * d1)) / (t1 * t1)
            - A2 * (2.0 * (p.y - c2) / (d2 * d2)) / (t2 * t2);

        return Point(-dfdx, -dfdy);
    }
    string name() override { return "Variant4 (Max)"; }
};

class QuadraticFunction : public Function
{
public:
    double operator()(const Point& p) override {
        return 100.0 * pow(p.y - p.x, 2) + pow(1.0 - p.x, 2);
        //100(y-x)^2+(1-x)^2
    }
    Point grad(const Point& p) override {
        double dfdx = 200.0 * (p.x - p.y) - 2.0 * (1.0 - p.x);
        double dfdy = 200.0 * (p.y - p.x);
        return Point(dfdx, dfdy);
    }
    string name() override { return "Quadratic"; }
};