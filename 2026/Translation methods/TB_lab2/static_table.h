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
				table.push_back({ terminals::EXPR_OPERATOR, word });
			if (type == "ASGN_OPERATOR")
				table.push_back({ terminals::ASGN_OPERATOR, word });
			if (type == "OPERATOR_CH")
				table.push_back({ terminals::OPERATOR_CH, word });
			if (type == "DELIMITER")
				table.push_back({ terminals::DELIMITER, word });
			if (type == "KEYWORD")
				table.push_back({ terminals::KEYWORD, word });

		}
		input_stream.close();
	}
	int contains(std::string word)
	{
		for (int i =0;i<table.size();i++)
		{
			if (table[i].second == word)
				return i;
		}
		return -1;
	}
};