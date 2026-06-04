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

	std::map<int, bool> is_nullable;

	std::map<int, std::set<symbol>> first_s;

	std::map<int, std::set<symbol>> follow_s;

	void parse(std::string file_name)
	{
		std::ifstream input_stream(file_name);
		
		std::vector<std::vector<symbol>> nonterminal_rules = {};
		std::vector<symbol> rule = {};
		// static table nonterminal index
		int current_nonterminal = 0;
		bool got_left_hand_side = false;
		states current_state = STATE_S;
		char input_character;

		while (!input_stream.eof())
		{
			switch (current_state)
			{
			case STATE_S:
				input_character = input_stream.get();
				if (std::isupper(input_character))
				{
					current_state = STATE_NONTERMINAL;
					break;
				}
				if (input_character == '\'')
				{
					current_state = STATE_TERMINAL;
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

			case STATE_NONTERMINAL:
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
				current_state = STATE_S;
				break;
			}
			case STATE_TERMINAL:
			{
				std::string terminal = "";
				while (input_stream.peek() != '\'')
				{
					input_character = input_stream.get();
					terminal += input_character;
				}
				// get rid of the closing apostrophe
				input_stream.get();
				int terminal_index = static_table.find(terminal);
				symbol s = { static_table.at(terminal_index).first, terminal_index};
				rule.push_back(s);
				current_state = STATE_S;
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
				current_state = STATE_S;
				break;
			}
			case ENDRULE:
				nonterminal_rules.push_back(rule);
				rule.clear();
				current_state = STATE_S;
				break;
			case ENDSECTION:
				grammar.insert({ current_nonterminal, nonterminal_rules });
				current_nonterminal = 0;
				nonterminal_rules.clear();
				current_state = STATE_S;
				got_left_hand_side = false;
				break;
			}
		}
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
				int current_nonterminal = nonterminal_production_rules.first;
				if (is_decided[current_nonterminal])
					continue;
				bool all_rules_non_nullable = true;
				for (const auto& rule : nonterminal_production_rules.second)
				{
					bool all_symbols_nullable = true;
					for (const auto& symbol : rule)
					{
						if (symbol.type == symbol_type::EPSILON)
							continue;
						if (symbol.type != symbol_type::NONTERMINAL)
						{
							all_symbols_nullable = false;
							break;
						}
						if (!is_decided[symbol.index])
						{
							all_symbols_nullable = false;
							all_rules_non_nullable = false;
							break;
						}
						if(!is_nullable[symbol.index])
						{
							all_symbols_nullable = false;
							break;
						}
					}
					if (all_symbols_nullable)
					{
						is_decided[current_nonterminal] = true;
						is_nullable[current_nonterminal] = true;
						all_rules_non_nullable = false;
					}
				}
				if (all_rules_non_nullable)
				{
					is_decided[current_nonterminal] = true;
					is_nullable[current_nonterminal] = false;
				}
			}

			bool all_decided = true;
			for (const auto& bucket : is_decided)
			{
				if (bucket.second == false)
				{
					all_decided = false;
					break;
				}
			}
			if (all_decided == true)
				break;
		}
	}
	
	void build_first()
	{
		std::map<int, bool> is_decided;
		// initialize all nonterminals as undecided
		// and initialize first_s
		for (const auto& bucket : grammar)
		{
			is_decided.insert({ bucket.first, false });
			first_s.insert({ bucket.first, std::set<symbol>{} });
		}

		while (true)
		{
			for (const auto& nonterminal_production_rules : grammar)
			{
				int current_nonterminal = nonterminal_production_rules.first;
				if (is_decided[current_nonterminal])
					continue;

				bool set_complete = true;

				for (const auto& rule : nonterminal_production_rules.second)
				{
					for (const auto& symbol : rule)
					{
						// if terminal, add it to the set and move onto the next rule
						if (symbol.type == symbol_type::EPSILON)
							break;
						if (symbol.type != symbol_type::NONTERMINAL)
						{
							first_s[current_nonterminal].insert(symbol);
							break;
						}
						// if nonterminal and its set is complete, add it to the set
						if (is_decided[symbol.index])
						{
							for (const auto& s : first_s[symbol.index])
							{
								first_s[current_nonterminal].insert(s);
							}
						}
						else
						{
							set_complete = false;
						}
						if (!is_nullable[symbol.index])
							break;
					}
				}
				if (set_complete)
				{
					is_decided[current_nonterminal] = true;
				}
			}

			bool all_decided = true;
			for (const auto& bucket : is_decided)
			{
				if (bucket.second == false)
				{
					all_decided = false;
					break;
				}
			}
			if (all_decided == true)
				break;
		}
	}

	void build_follow()
	{
		std::map<int, bool> is_decided;
		// initialize all nonterminals as undecided
		// and initialize follow_s
		for (const auto& bucket : grammar)
		{
			is_decided.insert({ bucket.first, false });
			follow_s.insert({ bucket.first, std::set<symbol>{} });
		}

		int starting_symbol_index = static_table.find("S");
		is_decided[starting_symbol_index] = true;
		follow_s[starting_symbol_index].insert(symbol{ symbol_type::ENDOFFILE, 0 });

		while (true)
		{
			// loop for going over all nonterminals
			for (const auto& nonterminals : grammar)
			{
				int current_nonterminal = nonterminals.first;
				if (is_decided[current_nonterminal])
					continue;
				bool set_complete = true;
				// loop for going over all production rules
				for (const auto& nonterminal_production_rules : grammar)
				{
					for (const auto& rule : nonterminal_production_rules.second)
					{
						int left_side_nonterminal = nonterminal_production_rules.first;
						bool past_target_nonterminal = false;
						for (auto it = rule.begin(); it != rule.end(); ++it) {
							const auto& symbol = *it;
							auto next_it = std::next(it);
							bool is_last = (next_it == rule.end());

							if (symbol.index == current_nonterminal)
							{
								past_target_nonterminal = true;
								if (is_last)
								{
									if (left_side_nonterminal == current_nonterminal)
										continue;
										//break;
										
									if (!is_decided[left_side_nonterminal])
									{
										set_complete = false;
										break;
									}
									for (const auto& s : follow_s[left_side_nonterminal])
									{
										follow_s[current_nonterminal].insert(s);
									}
								}
								//?
								continue;
							}
							// go next symbol if we havent got target nonterminal
							if (!past_target_nonterminal)
								continue;

							// if our nonterminal is followed by a terminal, add it and go next rule
							if (symbol.type != symbol_type::NONTERMINAL && symbol.type != symbol_type::EPSILON)
							{
								follow_s[current_nonterminal].insert(symbol);
								break;
							}

							if (symbol.type == symbol_type::NONTERMINAL)
							{
								for (const auto& s : first_s[symbol.index])
								{
									follow_s[current_nonterminal].insert(s);
								}

								if (!is_nullable[symbol.index])
									break;

								if (is_last)
								{
									if (!is_decided[left_side_nonterminal])
									{
										set_complete = false;
										break;
									}
									for (const auto& s : follow_s[left_side_nonterminal])
										follow_s[current_nonterminal].insert(s);
								}
							}
						}
					}
				}
				if (set_complete)
					is_decided[current_nonterminal] = true;
			}

			bool all_decided = true;
			for (const auto& bucket : is_decided)
			{
				if (bucket.second == false)
				{
					all_decided = false;
					break;
				}
			}
			if (all_decided == true)
				break;
		}
	}

	void build_parsing_table()
	{
		for (const auto& nonterminal_production_rules : grammar)
		{
			int current_nonterminal = nonterminal_production_rules.first;
			if (is_nullable[current_nonterminal])
			{
				for (const auto& s : follow_s[current_nonterminal])
				{
					int eps_index = static_table.find("eps");
					std::vector<symbol> eps_rule = { symbol{symbol_type::EPSILON, eps_index} };
					if (parsing_table.contains({ current_nonterminal, s }))
					{
						std::cerr << "non ll1";
						exit(0);
					}
					parsing_table[{current_nonterminal, s}] = eps_rule;
				}
			}
			for (const auto& rule : nonterminal_production_rules.second)
			{
				for (const auto& symbol : rule)
				{
					if (symbol.type != symbol_type::NONTERMINAL && symbol.type != symbol_type::EPSILON)
					{
						if (parsing_table.contains({ current_nonterminal, symbol }))
						{
							std::cerr << "non ll1";
							exit(0);
						}
						parsing_table[{current_nonterminal, symbol}] = rule;
						break;
					}
					if (symbol.type == symbol_type::NONTERMINAL)
					{
						for (const auto& s : first_s[symbol.index])
						{
							if (parsing_table.contains({ current_nonterminal, s }))
							{
								std::cerr << "non ll1";
								exit(0);
							}
							parsing_table[{current_nonterminal, s}] = rule;
						}
						if (!is_nullable[symbol.index])
							break;
					}
				}
			}
		}
	}

	std::string readable(symbol sym)
	{
		std::string s;
		int type = sym.type;
		if (type == symbol_type::ID)
			s = "id";
		else if (type == symbol_type::CONST_ID)
			s = "const_id";
		else if (type == symbol_type::INT_LITERAL)
			s = "int_literal";
		else if (type == symbol_type::EXPR_OPERATOR)
			s = "operator";
		else if (type == symbol_type::EPSILON)
			s = "eps";
		else if (type == symbol_type::ENDOFFILE)
			s = "#";
		else
			s = static_table.at(sym.index).second;
		return s;
	}

	enum states {
		STATE_S,
		// NONTERMINAL
		STATE_NONTERMINAL,
		// 'terminal'
		STATE_TERMINAL,
		// id, const_id, int_literal
		INT,
		// |
		ENDRULE,
		// ;
		ENDSECTION
	};
public:

	GrammarParser() : static_table("static_characters.txt")
	{
		parse("grammar_for_parsing.txt");
		build_nullable();
		build_first();
		build_follow();
		build_parsing_table();
	};

	void print_nullable()
	{
		for (const auto& bucket : is_nullable)
		{
			std::cout << std::boolalpha;
			std::cout << bucket.first << "\t" << bucket.second << "\n";
		}
		std::cout << "\n";
	}

	void print_first()
	{
		for (const auto& bucket : first_s)		{
			std::cout << bucket.first << "|\t";
			for (const auto& symbol : bucket.second)
			{
				std::cout << readable(symbol) << "\t";
			}
			std::cout << "|" << bucket.second.size() << '\n';
		}
		std::cout << '\n';
	}

	void print_follow()
	{
		for (const auto& bucket : follow_s)
		{
			std::cout << bucket.first << "|\t";
			for (const auto& symbol : bucket.second)
			{
				std::cout << readable(symbol) << "\t";
			}
			std::cout << "|" << bucket.second.size() << '\n';
		}
		std::cout << '\n';
	}

	void print_parsing_table()
	{
		for (const auto& bucket : parsing_table)
		{
			std::cout << static_table.at(bucket.first.first).second << ", " << readable(bucket.first.second) << ":=\t";
			for (const auto& symbol : bucket.second)
			{
				std::cout << readable(symbol) << '\t';
			}
			std::cout << '\n';
		}
		std::cout << "TABLE SIZE: " << parsing_table.size() << '\n';
		std::cout << '\n';
	}

	std::map<std::pair<int, symbol>, std::vector<symbol>> get_parsing_table()
	{
		return parsing_table;
	}
};
