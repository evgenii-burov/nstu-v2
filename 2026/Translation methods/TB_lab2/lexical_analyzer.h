#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include "static_table.h"
#include "dynamic_table.h"
#include "grammar.h"

class LexicalAnalyzer {
private:
	StaticTable static_table;

	DynamicTable dynamic_table;

	std::vector<symbol> tokens;

	enum states { S, ID, NUMBER, CONST, DELIMITER, OPERATOR, ERROR };

	std::set<char> operator_character_table{ '+','-', '*', '=', '!', '<'};
	std::set<char> delimiter_table{ ';',':','(',')','{','}' };

	bool is_id_start(char ch) {
		return std::islower(ch) || (ch == '_');
	}

	bool is_whitespace(char ch) {
		return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\0';
	}

	bool is_id_character(char ch) {
		return std::islower(ch) || std::isupper(ch) || std::isdigit(ch) || ch == '_';
	}

	bool is_delimiter(char ch) {
		return delimiter_table.contains(ch);
	}

	bool is_operator_character(char ch) {
		return operator_character_table.contains(ch);
	}

	bool is_const_character(char ch) {
		return std::isupper(ch) || std::isdigit(ch) || ch == '_';
	}

	bool is_operator_lexeme(std::string s) {
		return static_table.find(s) != -1;
	}

	bool is_legal_symbol(char ch) {
		return std::isupper(ch) ||
			std::islower(ch) ||
			std::isdigit(ch) ||
			is_operator_character(ch) ||
			is_delimiter(ch) ||
			is_whitespace(ch);
	}

public:
	LexicalAnalyzer()
		: static_table("static_characters.txt"),
		tokens({})
	{};

	void tokenize(std::string file_name) {
		std::ifstream input_stream(file_name);

		char ch;
		std::string error_sequence;
		std::string error_msg = "default_error";
		std::pair<int, int> error_pos;

		int cur_line = 1;
		int cur_offset = 0;

		enum states current_state = S;

		while (!input_stream.eof()) {
			switch (current_state) {
			case S:
			{
				ch = input_stream.get();
				//std::cout << "~" << ch << '\n';
				cur_offset++;
				while (is_whitespace(ch)) {
					if (ch == '\n') {
						cur_line++;
						cur_offset = 0;
					}
					ch = input_stream.get();
					//std::cout << "~" << ch << '\n';
					cur_offset++;
				}

				if (is_id_start(ch)) {
					current_state = ID;
				}

				else if (std::isdigit(ch)){
					current_state = NUMBER;
				}

				else if (std::isupper(ch)) {
					current_state = CONST;
				}

				else if (is_delimiter(ch)) {
					current_state = DELIMITER;
				}

				else if (is_operator_character(ch)) {
					current_state = OPERATOR;
				}

				else {
					current_state = ERROR;
					error_sequence = ch;
					error_msg = "Unknown symbol";
					error_pos = { cur_line, cur_offset };
					while (!is_legal_symbol(input_stream.peek()) && !input_stream.eof())
					{
						input_stream.get();
					}
				}
				break;
			}
			case ID:
			{
				std::string identifier(1, ch);
				while (is_id_character(input_stream.peek())) {
					ch = input_stream.get(); // ch = ??????
					//std::cout << "~" << ch << '\n';
					cur_offset++;
					identifier += ch;
				}

				//process identifier
				int identifier_index = static_table.find(identifier);
				if (identifier_index == -1) {
					identifier_index = dynamic_table.insert(identifier, "int", symbol_type::ID, "");
					tokens.push_back(symbol{ symbol_type::ID, identifier_index });
				}
				else {
					tokens.push_back(symbol{ symbol_type::KEYWORD, identifier_index });
				}

				current_state = S;
				break;
			}
			case NUMBER:
			{
				if (ch == '0' && std::isdigit(input_stream.peek())) {
					current_state = ERROR;
					error_sequence = ch;
					break;
				}
				std::string number(1, ch);
				while (std::isdigit(input_stream.peek())) {
					ch = input_stream.get();
					//std::cout << "~" << ch << '\n';
					cur_offset++;
					number += ch;
				}
				if (is_id_character(input_stream.peek())) {	// checks for letters, underscores and digits
					ch = input_stream.get();				// (shouldn't be a digit since the
					//std::cout << "~" << ch << '\n';
					cur_offset++;							// above while loop would've processed it)
					current_state = ERROR;
					error_sequence = ch;
					error_msg = "Expected a number";
					error_pos = { cur_line, cur_offset-1 };
					while (is_id_character(input_stream.peek())) {
						input_stream.get();
					}
					break;
				}

				//process number
				int number_index = dynamic_table.insert(number, "int", symbol_type::INT_LITERAL, number);
				tokens.push_back(symbol{ symbol_type::INT_LITERAL, number_index});

				current_state = S;
				break;
			}
			case CONST:
			{
				std::string const_str(1, ch);
				while (is_const_character(input_stream.peek())) {
					ch = input_stream.get();
					//std::cout << "~" << ch << '\n';
					cur_offset++;
					const_str += ch;
				}
				if (std::islower(input_stream.peek())) {
					current_state = ERROR;
					error_sequence = ch;
					error_msg = "Const identifier must consist of uppercase letters and \'_\' ";
					error_pos = { cur_line, cur_offset - 1 };
					while (std::islower(input_stream.peek())) {
						input_stream.get();
					}
					break;
				}

				//process const
				int const_index = dynamic_table.insert(const_str, "int", symbol_type::CONST_ID, "");
				tokens.push_back(symbol{ symbol_type::CONST_ID, const_index });

				current_state = S;
				break;
			}
			case DELIMITER:
			{
				//process delimiter
				int delimiter_index = static_table.find(std::string(1, ch));
				tokens.push_back({ symbol_type::DELIMITER, delimiter_index });

				current_state = S;
				break;
			}
			case OPERATOR:
			{
				std::string operator_str(1, ch);
				while (is_operator_character(input_stream.peek())) {
					ch = input_stream.get();
					operator_str += ch;
					cur_offset++;
				}
				int operator_index = static_table.find(operator_str);
				if(operator_index == -1) {
					current_state = ERROR;
					error_msg = "Unknown operator";
					error_pos = { cur_line, cur_offset - operator_str.size() };
					break;
				}
				else
				{
					tokens.push_back(symbol{ static_table.at(operator_index).first, operator_index});
				}
				current_state = S;
				break;
			}
			case ERROR:
			{
				std::cerr << "Error: " << error_msg << " at position (" << cur_line << ", " << cur_offset << ")\n\n";
				current_state = S;
				break;
			}
			}
		}
		input_stream.close(); //finished tokenizing

		std::ofstream output_stream("identifier_table.txt");
		dynamic_table.output_table(output_stream);
		output_stream.close();

		output_stream.open("tokens.txt");
		for (const auto token : tokens) {
			output_stream << token.type << '\t' << token.index << '\n';
		}
		output_stream.close();
	}
};