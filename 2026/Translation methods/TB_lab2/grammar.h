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
	ENDOFFILE,
	ACTION
};

enum action_id {
	A_DECLARE, A_INIT_ADDR, A_PUSH_ADDR, A_PUSH_VAR, A_PUSH_CONST,
	A_EXPR_OPEN, A_OP, A_NEG, A_EXPR_CLOSE, A_ASSIGN,
	A_CONST_BEGIN, A_CONST_END, A_DECL_CONST, A_SET_CONST,
	A_SW_BEGIN, A_CASE_TEST, A_DEFAULT, A_BRK, A_SW_END
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

	friend std::ostream& operator<<(std::ostream& os, const symbol& s)
	{
		os << s.type << " " << s.index;
		return os;
	}
};

std::set<int> NONSPECIFIC_SYMBOLS = std::set<int>({ ID, CONST_ID, INT_LITERAL, EXPR_OPERATOR, EPSILON, ENDOFFILE });


const int NONSPECIFIC_INDEX = -1;

//struct SymbolKeyCompare {
//	bool operator()(const std::pair<int, symbol>& a,
//		const std::pair<int, symbol>& b) const
//	{
//		if (a.first != b.first) return a.first < b.first;
//		if (a.second.index == -1 || b.second.index == -1) return false;
//		if (a.second.type != b.second.type) return a.second.type < b.second.type;
//		return a.second.index < b.second.index;
//	}
//};

using ParsingTable = std::map<std::pair<int, symbol>, std::vector<symbol>>;

inline ParsingTable PARSING_TABLE{};
