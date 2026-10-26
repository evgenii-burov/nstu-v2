#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <queue>
#include "grammar.h"

bool compare_symbols(symbol s1, symbol s2)
{
	//std::cout << "$" << s1 << "|" << s2 << "$\n";
	if (NONSPECIFIC_SYMBOLS.contains(s1.type) && NONSPECIFIC_SYMBOLS.contains(s2.type))
	{
		return s1.type == s2.type;
	}
	return s1.type == s2.type && s1.index == s2.index;
}

class SyntaxAnalyzer
{
private:
	const StaticTable& static_table;

	const ParsingTable& parsing_table;

	std::string readable(symbol sym)
	{
		std::string s;
		int type = sym.type;

		switch (type)
		{
		case symbol_type::ID:
			s = "id";
			break;
		case symbol_type::INT_LITERAL:
			s = "int_literal";
			break;
		case symbol_type::CONST_ID:
			s = "const_id";
			break;
		case symbol_type::EXPR_OPERATOR:
			s = "operator";
			break;
		case symbol_type::EPSILON:
			s = "eps";
			break;
		case symbol_type::ENDOFFILE:
			s = "eof";
			break;
		default:
			s = static_table.at(sym.index).second;
			break;
		}
		return s;
	}
public:

	SyntaxAnalyzer()
		: parsing_table(PARSING_TABLE), static_table(STATIC_TABLE)
	{}


	void parse(std::string file_name)
	{
		std::ifstream input_stream(file_name);

		std::queue<symbol> tokens;

		int token_index = 0;
		int token_type = 0;

		while (input_stream >> token_type >> token_index)
		{
			tokens.push({ token_type, token_index });
			std::cout << "token:" << readable({ token_type, token_index }) << '\n';
		}
		tokens.push(symbol{ symbol_type::ENDOFFILE, -1 });

		std::stack<symbol> parse_stack{};

		parse_stack.push(symbol{ symbol_type::ENDOFFILE, -1 });
		parse_stack.push(symbol{ symbol_type::NONTERMINAL, static_table.find("S")});

		int count = 0;
		while (!compare_symbols(parse_stack.top(), symbol{ ENDOFFILE, -1 }))
		{
			count++;
			std::cout << count << " stack:\t" << readable(parse_stack.top()) << '\t';
			std::cout << "token:" << readable(tokens.front()) << '\n';
			// terminal symbol
			if (parse_stack.top().type != NONTERMINAL)
			{
				if (compare_symbols(parse_stack.top(), tokens.front()))
				{
					parse_stack.pop();
					tokens.pop();
					std::cout << "pop\n";
				}
				else
				{
					std::cerr << "Expected symbol: " << readable(parse_stack.top()) << '\n';
					exit(0);
				}
			}
			// nonterminal symbol
			else
			{ 
				// if the token is nonspecific
				int index_to_use;
				if (NONSPECIFIC_SYMBOLS.count(tokens.front().type) == 1)
				{
					bool nonspecific = true;
					if (parsing_table.contains({ parse_stack.top().index, symbol{ tokens.front().type, NONSPECIFIC_INDEX } }))
					{
						index_to_use = NONSPECIFIC_INDEX;
					}
					else
					{
						if (parsing_table.contains({ parse_stack.top().index, symbol{ tokens.front().type, tokens.front().index }}))
						{
							index_to_use = tokens.front().index;
						}
						else
						{
							std::cerr << "No usable rule\n";
							exit(0);
						}
					}
				}
				else
				{
					index_to_use = tokens.front().index;
				}

				if (!parsing_table.contains({ parse_stack.top().index, symbol{tokens.front().type, index_to_use}}))
				{
					std::cerr << "No usable rule\n";
					exit(0);
				}

				symbol stack_top = parse_stack.top();
				parse_stack.pop();

				std::vector<symbol> rule = parsing_table.at({ stack_top.index, symbol{tokens.front().type, index_to_use} });

				if (compare_symbols(rule[0], symbol{symbol_type::EPSILON, NONSPECIFIC_INDEX }))
					continue;

				for (int i = rule.size() - 1; i >= 0; i--)
				{
					parse_stack.push(rule[i]);
				}
			}
		}
		std::cout << count << " stack:\t" << readable(parse_stack.top()) << '\t';
		std::cout << "token:" << readable(tokens.front()) << '\n';
		//if (!compare_symbols(parse_stack.top(), tokens.front()) || !(tokens.front().type == symbol_type::ENDOFFILE))
		//{
		//	std::cerr << "End of file error";
		//}
	}
};