#pragma once
#include "grammar.h"
#include "static_table.h"
#include <vector>
#include <iostream>

class GrammarParser
{
private:
	StaticTable static_table;

	std::map<int, std::vector<std::vector<symbol>>> grammar;

	std::map<int, bool> nullability;

	bool is_nullable(int nonterminal_index)
	{
		return nullability.at(nonterminal_index);
	}

	void build_nullable()
	{
		std::map<int, bool> is_decided;
		// initialize all nonterminals as undecided
		for (const auto& bucket : grammar)
		{
			is_decided.insert({ bucket.first, false });
		}

		while (true)
		{
			for (const auto& nonterminal_production_rules : grammar)
			{
				for (const auto& rule : nonterminal_production_rules.second)
				{

				}
			}

			bool all_decided;
			for (const auto& bucket : is_decided)
			{
				if (bucket.second == false)
				{
					all_decided = false;
					break;
				}
				all_decided = true;
			}
			if (all_decided == true)
				break;
		}
	}

	enum states {
		S,
		// NONTERMINAL
		NONTERMINAL,
		// 'terminal'
		TERMINAL,
		// id, const_id, int_literal
		INT,
		// |
		ENDRULE,
		// ;
		ENDSECTION
	};
public:

	GrammarParser() : static_table("static_characters.txt") {};
	
	void parse(std::string file_name)
	{
		std::ifstream input_stream(file_name);
		
		std::map<int, std::vector<std::vector<symbol>>> grammar;
		std::vector<std::vector<symbol>> nonterminal_rules = {};
		std::vector<symbol> rule = {};
		// static table nonterminal index
		int current_nonterminal = 0;
		bool got_left_hand_side = false;
		states current_state = S;
		char input_character;

		while (!input_stream.eof())
		{
			switch (current_state)
			{
			case S:
				input_character = input_stream.get();
				if (std::isupper(input_character))
				{
					current_state = NONTERMINAL;
					break;
				}
				if (input_character == '\'')
				{
					current_state = TERMINAL;
					break;
				}
				if (std::islower(input_character))
				{
					current_state = INT;
					break;
				}
				if (input_character == '|')
				{
					current_state = ENDRULE;
					break;
				}
				if (input_character == ';')
				{
					current_state = ENDSECTION;
					break;
				}
				break;

			case NONTERMINAL:
			{
				std::string nonterminal = { input_character };
				while (std::isupper(input_stream.peek()) || input_stream.peek() == '_')
				{
					input_character = input_stream.get();
					nonterminal += input_character;
				}
				if (got_left_hand_side)
				{
					symbol s = { symbol_type::NONTERMINAL, static_table.find(nonterminal) };
					rule.push_back(s);
				}
				else
				{
					got_left_hand_side = true;
					current_nonterminal = static_table.find(nonterminal);
				}
				current_state = S;
				break;
			}
			case TERMINAL:
			{
				std::string terminal = "";
				while (input_stream.peek() != '\'')
				{
					input_character = input_stream.get();
					terminal += input_character;
				}
				// get rid of the closing apostrophe
				input_stream.get();
				std::cout << "T" << terminal << "T\n";
				int terminal_index = static_table.find(terminal);
				std::cout << "T" << terminal_index << "T\n";
				symbol s = { static_table.at(terminal_index).first, terminal_index};
				rule.push_back(s);
				current_state = S;
				break;
			}
			case INT:
			{
				std::string int_token = { input_character };
				while (std::islower(input_stream.peek()) || input_stream.peek() == '_')
				{
					input_character = input_stream.get();
					int_token += input_character;
				}
				symbol s = {-1, -1};
				if (int_token == "id")
					s = { symbol_type::ID, 0 };
				if (int_token == "const_id")
					s = { symbol_type::CONST_ID, 0 };
				if (int_token == "int_literal")
					s = { symbol_type::INT_LITERAL, 0 };
				if (int_token == "operator")
					s = { symbol_type::EXPR_OPERATOR, 0 };
				rule.push_back(s);
				current_state = S;
				break;
			}
			case ENDRULE:
				nonterminal_rules.push_back(rule);
				rule.clear();
				current_state = S;
				break;
			case ENDSECTION:
				grammar.insert({ current_nonterminal, nonterminal_rules });
				current_nonterminal = 0;
				nonterminal_rules.clear();
				current_state = S;
				got_left_hand_side = false;
				break;
			}
		}
	}

};


class TableBuilder {
private:


	std::map<int, bool> nullability;

	bool is_nullable(int nonterminal_index)
	{
		return nullability.at(nonterminal_index);
	}

	void build_nullable()
	{
		std::map<int, bool> is_decided;

	}
public:

};
