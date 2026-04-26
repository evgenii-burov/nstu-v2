#pragma once
#include <iostream>
#include <string>
#include <fstream>

class LexicalAnalyzer {
private:
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
		//TODO: delimiter table
		return false;
	}

	bool is_operator(char ch) {
		//TODO: operator table
		return false;
	}

public:
	LexicalAnalyzer();

	void tokenize(std::string file_name) {
		std::ifstream input_stream(file_name);

		char ch;
		char error_ch;

		enum states current_state = S;

		while (!input_stream.eof()) {
			switch (current_state) {
			case S:
			{
				ch = input_stream.get();
				while (is_whitespace(ch)) {
					ch = input_stream.get();
				}

				if (is_id_start(ch)) {
					current_state = ID;
				}

				else if (std::isdigit(ch)) {
					current_state = NUMBER;
				}

				else if (std::isupper(ch)) {
					current_state = CONST;
				}

				else if (is_delimiter(ch)) {
					current_state = DELIMITER;
				}

				else if (is_operator(ch)) {
					current_state = OPERATOR;
				}

				else {
					current_state = ERROR;
				}
				break;
			}
			case ID:
			{
				std::string identifier(1, ch);
				ch = input_stream.peek();
				while (is_id_character(ch)) {
					input_stream.get();
					identifier += ch;
				}
				//TODO: process identifier
				current_state = S;
				break;
			}
			case NUMBER:
			{

				break;
			}
			case CONST:
			{
				break;
			}
			case DELIMITER:
			{
				break;
			}
			case OPERATOR:
			{
				break;
			}
			case ERROR:
			{
				break;
			}
			}
		}
	}
};