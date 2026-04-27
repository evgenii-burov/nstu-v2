#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "static_table.h"
#include "dynamic_table.h"

struct Token {
	std::string token;
	std::string type;
};

class LexicalAnalyzer {
private:
	StaticTable delimiter_table, operator_lexeme_table, operator_character_table, keyword_table;
	DynamicTable identifier_table, const_table;

	std::vector<Token> tokens;

	enum states { S, ID, NUMBER, CONST, DELIMITER, OPERATOR, ERROR };

	bool is_id_start(char ch) {
		return std::islower(ch) || (ch == '_');
	}

	bool is_whitespace(char ch) {
		return ch == ' ' || ch == '\n' || ch == '\t';
	}

	bool is_id_character(char ch) {
		return std::islower(ch) || std::isupper(ch) || std::isdigit(ch) || ch == '_';
	}

	bool is_delimiter(char ch) {
		return delimiter_table.contains(std::string(1,ch));
	}

	bool is_operator_character(char ch) {
		return operator_character_table.contains(std::string(1, ch));
	}

	bool is_const_character(char ch) {
		return std::isupper(ch) || std::isdigit(ch) || ch == '_';
	}

	bool is_operator_lexeme(std::string s) {
		return operator_lexeme_table.contains(s);
	}

public:
	LexicalAnalyzer(std::string file_name)
		: delimiter_table(file_name, "DELIMITER"),
		operator_character_table(file_name, "OPERATOR_CH"),
		operator_lexeme_table(file_name, "OPERATOR_LX"),
		keyword_table(file_name, "KEYWORD"),
		identifier_table(std::string("Identifiers")),
		const_table(std::string("Consts")),
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
				std::cout << "~" << ch << '\n';
				cur_offset++;
				while (is_whitespace(ch)) {
					if (ch == '\n') {
						cur_line++;
						cur_offset = 0;
					}
					ch = input_stream.get();
					std::cout << "~" << ch << '\n';
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
				}
				break;
			}
			case ID:
			{
				std::string identifier(1, ch);
				while (is_id_character(input_stream.peek())) {
					ch = input_stream.get(); // ch = ??????
					std::cout << "~" << ch << '\n';
					cur_offset++;
					identifier += ch;
				}

				//process identifier
				if (keyword_table.contains(identifier)) {
					tokens.push_back(Token{ identifier, std::string("KEYWRD") });
				}
				else {
					tokens.push_back(Token{ identifier, std::string("ID") });
					lexeme lx{ identifier, std::string("int") };
					identifier_table.insert(identifier, "int");
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
					std::cout << "~" << ch << '\n';
					cur_offset++;
					number += ch;
				}
				if (is_id_character(input_stream.peek())) {	// checks for letters, underscores and digits
					ch = input_stream.get();				// (shouldn't be a digit since the
					std::cout << "~" << ch << '\n';
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
				tokens.push_back(Token{ number, std::string("CONST") });
				lexeme lx{ number, std::string("int") };
				identifier_table.insert(number, "int");

				current_state = S;
				break;
			}
			case CONST:
			{
				std::string const_str(1, ch);
				while (is_const_character(input_stream.peek())) {
					ch = input_stream.get();
					std::cout << "~" << ch << '\n';
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
				tokens.push_back(Token{ const_str, std::string("CONST") });
				lexeme lx{ const_str, std::string("int") };
				const_table.insert(const_str, "int");

				current_state = S;
				break;
			}
			case DELIMITER:
			{
				//process delimiter
				tokens.push_back({ std::string(1,ch), "DELIM" });

				current_state = S;
				break;
			}
			case OPERATOR:
			{
				std::string operator_str(1, ch);
				if (is_operator_character(input_stream.peek())) {
					ch = input_stream.get();
					std::cout << "~" << ch << '\n';
					cur_offset++;
					operator_str += ch;
				}
				if (is_operator_lexeme(operator_str) && !is_operator_character(input_stream.peek())) {
					//process operator
					tokens.push_back({ operator_str, "OPER" });
				}
				else {
					current_state = ERROR;
					error_sequence = ch;
					error_msg = "Unknown operator";
					break;
				}
				current_state = S;
				break;
			}
			case ERROR:
			{
				std::cerr << "Error: " << error_msg << " at position (" << cur_line << ", " << cur_offset << ")\n";
				current_state = S;
				break;
			}
			}
		}
		input_stream.close(); //finished tokenizing

		identifier_table.output_table(std::cout);
		const_table.output_table(std::cout);

		std::cout << "Tokens:\n";
		std::cout << "TOKEN\t" << "TKN_TYPE\n";
		for (const auto token : tokens) {
			std::cout << token.token << '\t' << token.type << '\n';
		}
	}
};