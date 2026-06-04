#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <queue>
#include <set>
#include "grammar.h"
#include "grammar_parser.h"

std::set<int> nonspecific_symbols{ID, CONST_ID, INT_LITERAL, EXPR_OPERATOR, EPSILON, ENDOFFILE};

bool compare_symbols(symbol s1, symbol s2)
{
	if (nonspecific_symbols.contains(s1.type) && nonspecific_symbols.contains(s2.type))
	{
		return s1.type == s2.type;
	}
	return s1.type == s2.type && s1.index == s2.index;
}

class SyntaxAnalyzer
{
private:
	StaticTable static_table = StaticTable("static_characters.txt");

	std::stack<symbol> parse_stack;

	std::map<std::pair<int, symbol>, std::vector<symbol>> parsing_table;
public:

	SyntaxAnalyzer(std::map<std::pair<int, symbol>, std::vector<symbol>> table) : parsing_table(table) {};

	void parse(std::string file_name)
	{
		std::ifstream input_stream(file_name);

		std::queue<symbol> tokens;

		int token_index = 0;
		int token_type = 0;

		while (!input_stream.eof())
		{
			input_stream >> token_type >> token_index;
			tokens.push({ token_type, token_index });
		}
		tokens.push(symbol{ symbol_type::ENDOFFILE, -1 });

		parse_stack.push(symbol{ symbol_type::ENDOFFILE, -1 });
		parse_stack.push(symbol{ symbol_type::NONTERMINAL, static_table.find("S")});

		while (!compare_symbols(parse_stack.top(), symbol{ ENDOFFILE, -1 }))
		{
			std::cout << parse_stack.top().type << "&" << parse_stack.top().index;
			// terminal symbol
			if (parse_stack.top().type != NONTERMINAL)
			{
				if (compare_symbols(parse_stack.top(), tokens.front()))
				{
					std::cout << "terminal:" << tokens.front().type << '&';
					parse_stack.pop();
					tokens.pop();
				}
				else
				{
					std::cerr << "Expected symbol: " << parse_stack.top().type << '&' << parse_stack.top().index << '\n';
					exit(0);
				}
			}
			// nonterminal symbol
			if (!parsing_table.contains({ parse_stack.top().index, tokens.front() }))
			{
				std::cerr << "No usable rule\n";
				exit(0);
			}
			symbol stack_top = parse_stack.top();
			parse_stack.pop();

			std::vector<symbol> rule = parsing_table[{stack_top.index, tokens.front()}];

			for (int i = rule.size(); i >= 0; i--)
			{
				parse_stack.push(rule[i]);
			}
		}
		if (!compare_symbols(parse_stack.top(), tokens.front()) || !(tokens.front().type == symbol_type::ENDOFFILE))
		{
			std::cerr << "End of file error";
		}
	}
};