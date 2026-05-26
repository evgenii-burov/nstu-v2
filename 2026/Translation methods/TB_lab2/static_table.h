#pragma once
#include <set>
#include <string>
#include <fstream>
#include "grammar.h"

class StaticTable
{
private:
	std::vector<std::pair<int, std::string>> table;

public:
	StaticTable(std::string file_name)
	{
		std::string type;
		std::string word;
		std::ifstream input_stream(file_name);
		while (!input_stream.eof())
		{
			input_stream >> type >> word;
			if (type == "EXPR_OPERATOR")
				table.push_back({ symbol_type::EXPR_OPERATOR, word });
			if (type == "ASGN_OPERATOR")
				table.push_back({ symbol_type::ASGN_OPERATOR, word });
			if (type == "OPERATOR_CH")
				table.push_back({ symbol_type::OPERATOR_CH, word });
			if (type == "DELIMITER")
				table.push_back({ symbol_type::DELIMITER, word });
			if (type == "KEYWORD")
				table.push_back({ symbol_type::KEYWORD, word });

		}
		input_stream.close();
	}
	// -1 if not found, otherwise index in the table
	int find(std::string word)
	{
		for (int i =0;i<table.size();i++)
		{
			if (table[i].second == word)
				return i;
		}
		return -1;
	}
	std::pair<int, std::string> at(int index)
	{
		return table[index];
	}
};