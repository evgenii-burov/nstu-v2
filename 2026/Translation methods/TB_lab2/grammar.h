#pragma once
#include <map>

enum symbol_type {
	ID,
	CONST_ID,
	INT_LITERAL,
	DELIMITER,
	EXPR_OPERATOR,
	ASGN_OPERATOR,
	OPERATOR_CH,
	KEYWORD,
	EPSILON,
	ENDOFFILE,

	S,
	BLOCK,
	LOOP_BLOCK,
	DECLARATION,
	OPTIONAL_INIT,
	ASSIGNMENT,
	CONST_ASSIGNMENT,
	EXPRESSION,
	EXPRESSION_TAIL,
	TERM,
	OPERAND,
	CONST_EXPRESSION,
	CONST_EXPRESSION_TAIL,
	CONST_TERM,
	CONST_OPERAND,
	SWITCH,
	SWITCH_BODY,
	CASES,
	CASE,
	DEFAULT
};

bool is_terminal(symbol_type s) {
	return s <= ENDOFFILE;
}

struct symbol {
	// corresponds to enum terminals
	int type;
	// index in the static table
	int index;
};

std::map<std::pair<symbol_type, symbol_type>, std::vector<symbol>> parsing_table;

