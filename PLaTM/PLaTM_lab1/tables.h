#pragma once
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>

struct Symbol
{
    std::string name;
    int value;
    std::string type;
    std::string attributes;
};

struct Constant
{
    int value;
    std::string type;
    std::string attributes;

    Constant(int value0, std::string type0, std::string attributes0) :
        value(value0), type(type0), attributes(attributes0)
    {
    };

    std::size_t get_hash()
    {
        std::size_t h1 = std::hash<int>{}(value);
        std::size_t h2 = std::hash<std::string>{}(type);
        std::size_t h3 = std::hash<std::string>{}(attributes);
        return h1 ^ (h2 << 1) ^ h3;
    }
};

class ConstantTable
{
private:
    std::set<std::string> table;
public:
    void load_table(std::string file_name);
    bool has(std::string element);
    int find(std::string element);
    std::string at_index(int index);
    void print_table();
};