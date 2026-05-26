#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include "grammar.h"

class SyntaxAnalyzer
{
private:
	std::stack<symbol> parse_stack;
public:
	void parse(std::string file_name)
	{
		std::ifstream input_stream(file_name);

		std::vector<symbol> tokens;

		int token_index = 0;
		int token_type = 0;

		while (!input_stream.eof())
		{
			input_stream >> token_type >> token_index;
			tokens.push_back({ token_type, token_index });
		}
		tokens.push_back(symbol{ symbol_type::ENDOFFILE, -1 });

		parse_stack.push(symbol{ symbol_type::ENDOFFILE, -1 });
		parse_stack.push(symbol{ symbol_type::S, -1 });

		while(parse_stack.top)
	}
}