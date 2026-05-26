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

bool is_terminal(int s) {
	return s <= ENDOFFILE;
}

// specifies any grammar symbol
struct symbol {
	// symbol type
	int type;
	// index in the corresponding table
	int index;
};

std::map<std::pair<symbol_type, symbol>, std::vector<symbol>> parsing_table;

