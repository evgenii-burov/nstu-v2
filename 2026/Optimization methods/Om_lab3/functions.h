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

class Penalty {
public:
    double penalty_coefficient = 1;
    virtual double operator()(const Point& p) = 0;
    virtual bool check(const Point& p) = 0;
};

class PenaltyA : public Penalty {
    /*
    y-x>=1
    x - y + 1 <= 0
    g(x,y) = x - y + 1

    G = ( 1/2 * ( g(x,y) + abs(g(x,y)))  ) 
    */
private:
    double g(const Point& p) {
        return p.x - p.y + 1;
    }
public:
    double operator()(const Point& p) override {
        return penalty_coefficient * pow((g(p) + abs(g(p)) / 2), 1);
    }
    bool check(const Point& p) override {
        return this->operator()(p) == 0.0;
    }
};

class Function {
public:
    Penalty& penalty;
    virtual double operator()(const Point& p) = 0;
    Function(Penalty& p) : penalty(p)
    {};
    virtual ~Function() {}
};

class PenaltyFunction : public Function {
public:
    using Function::Function;
    double operator()(const Point& p) override {
        return 2 * (p.x - p.y) * (p.x - p.y) + 14 * (p.y - 3) * (p.y - 3) + penalty(p);
    }
};
