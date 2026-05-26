#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include "grammar.h"

class SyntaxAnalyzer
{
private:
	std::stack<symbol_type> parse_stack;
public:
	void parse(std::string file_name)
	{
		std::ifstream input_stream(file_name);

		std::vector<token> tokens;

		int token_index = 0;
		int token_type = 0;

		while (!input_stream.eof())
		{
			input_stream >> token_type >> token_index;
			tokens.push_back({ token_type, token_index });
		}
		tokens.push_back({ terminals::ENDOFFILE, -1 });


	}
}