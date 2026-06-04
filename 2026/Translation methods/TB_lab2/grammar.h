#pragma once
#include <map>
#include <set>
#include "static_table.h"

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

std::set<int> nonspecific_symbols{ ID, CONST_ID, INT_LITERAL, EXPR_OPERATOR, EPSILON, ENDOFFILE };

StaticTable static_table = StaticTable("static_characters.txt");

struct SymbolKeyCompare {
	bool operator()(const std::pair<int, symbol>& a,
		const std::pair<int, symbol>& b) const {
		if (a.first != b.first) return a.first < b.first;
		if (a.second.type != b.second.type) return a.second.type < b.second.type;
		// -1 is wildcard: treat as equal to anything
		if (a.second.index == -1 || b.second.index == -1) return false;
		return a.second.index < b.second.index;
	}
};

std::map<std::pair<int, symbol>, std::vector<symbol>, SymbolKeyCompare> parsing_table;

