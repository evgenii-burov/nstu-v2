#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <stack>
#include <utility>
#include <queue>
#include <functional>

class IntDouble
{
public:
    int first;
    double second;

    IntDouble(int first0 = 0, double second0 = 0.0) : first(first0), second(second0) {}

    bool operator>(const IntDouble& other) const
    {
        return first > other.first;
    }

    bool operator==(const IntDouble& other) const
    {
        return first == other.first;
    }
};

using namespace std;

void array_container_compatibility();

void dynamic_array_task(int n, int value_range);

void stack_task(int n);

void priority_queue_task(int n);

void true_priority_queue_task(int n);