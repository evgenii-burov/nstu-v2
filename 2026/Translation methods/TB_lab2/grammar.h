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
	NONTERMINAL,
	ENDOFFILE
};

// specifies any grammar symbol,
// for some symbols like expr operator, due to the specifics of the grammar,
// only the type matters
struct symbol {
	// symbol type
	int type;
	// index in the corresponding table,
	// table contains pairs of symbol_types and corresponding symbol strings
	int index;

	bool operator<(const symbol& other) const
	{
		if (type != other.type)
			return type < other.type;
		return index < other.index;
	}
};

//std::map<int, std::vector<std::vector<symbol>>> grammar_rules;

//std::map<std::pair<symbol_type, symbol>, std::vector<symbol>> parsing_table;

