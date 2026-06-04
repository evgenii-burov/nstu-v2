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

	std::ostream& operator<<(std::ostream& out)
	{
		std::string s;
		if (type == symbol_type::ID)
			s = "id";
		if (type == symbol_type::CONST_ID)
			s = "const_id";
		if (type == symbol_type::INT_LITERAL)
			s = "int_literal";
		if (type == symbol_type::EXPR_OPERATOR)
			s = "operator";
		if (type == symbol_type::EPSILON)
			s = "eps";
		if (type == symbol_type::ENDOFFILE)
			s = "#";
	}
};

//std::map<int, std::vector<std::vector<symbol>>> grammar_rules;

//std::map<std::pair<symbol_type, symbol>, std::vector<symbol>> parsing_table;

