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
    ConstantTable(std::string file_name);
    bool has(std::string element);
    int find(std::string element);
    std::string at_index(int index);
    void print_table();
};

template <typename Type>
class Table
{
private:
    std::vector<std::vector<Type>> table;
public:
    Table()
    {
        table.resize(50);
    }
    void insert(Type element)
    {
        std::size_t hash = element.get_hash();
        table[hash % table.size()].push_back(element);
    }
    bool has(Type element)
    {
        std::size_t hash = element.get_hash();
        for (const auto& elem_at_index : table[hash % table.size()])
        {
            if (elem_at_index == element)
            {
                return true;
            }
        }
        return false;
    }
    int find(Type element)
    {

    }
    std::string at_index(int index);
    void print_table();
    void save_table(std::string file_name);
    void load_table(std::string file_name);
};