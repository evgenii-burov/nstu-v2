#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <queue>
#include <memory>
#include <string>
#include "grammar.h"
#include "dynamic_table.h"

bool compare_symbols(symbol s1, symbol s2)
{
	if (NONSPECIFIC_SYMBOLS.contains(s1.type) && NONSPECIFIC_SYMBOLS.contains(s2.type))
	{
		return s1.type == s2.type;
	}
	return s1.type == s2.type && s1.index == s2.index;
}

// узел дерева разбора
struct Node
{
	symbol sym;                 // нетерминал или терминал
	symbol tok{};               // для терминала: принятый токен
	std::vector<Node*> ch;
};

class SyntaxAnalyzer
{
private:
	const StaticTable& static_table;
	const ParsingTable& parsing_table;
	const DynamicTable& dynamic_table;

	std::vector<std::unique_ptr<Node>> pool;   // владеет всеми узлами
	Node* root = nullptr;

	std::vector<std::string> poliz;
	int label_cnt = 0;
	int tmp_cnt = 0;

	std::string readable(symbol sym)
	{
		std::string s;
		int type = sym.type;
		switch (type)
		{
		case symbol_type::ID:          s = "id"; break;
		case symbol_type::CONST_ID:    s = "const_id"; break;
		case symbol_type::INT_LITERAL: s = "int_literal"; break;
		case symbol_type::EXPR_OPERATOR: s = "operator"; break;
		case symbol_type::EPSILON:     s = "eps"; break;
		case symbol_type::ENDOFFILE:   s = "#"; break;
		case symbol_type::ACTION:      s = ACTIONS.at(sym.index).second; break;
		default:                       s = static_table.at(sym.index).second;
		}
		return s;
	}

	// ---------- разбор с построением дерева ----------

	Node* make_node(symbol s)
	{
		pool.push_back(std::make_unique<Node>());
		pool.back()->sym = s;
		return pool.back().get();
	}

	// ищет правило: сначала обобщённый ключ, потом точный
	const std::vector<symbol>* find_rule(int nonterminal, const symbol& tok)
	{
		if (NONSPECIFIC_SYMBOLS.count(tok.type) == 1)
		{
			auto it = parsing_table.find({ nonterminal, symbol{ tok.type, NONSPECIFIC_INDEX } });
			if (it != parsing_table.end())
				return &it->second;
		}
		auto it = parsing_table.find({ nonterminal, tok });
		if (it != parsing_table.end())
			return &it->second;
		return nullptr;
	}

	// ---------- текст токена ----------
	std::string text_of(const symbol& t)
	{
		switch (t.type)
		{
		case symbol_type::ID:
			return dynamic_table.at(t.index).name;
		case symbol_type::CONST_ID:
			return dynamic_table.at(t.index).name;
		case symbol_type::INT_LITERAL:
			return dynamic_table.at(t.index).name;
		default:
			return static_table.at(t.index).second;
		}
	}

	// ---------- вспомогательные для обхода ----------

	int NT(const char* name) { return static_table.find(name); }

	bool is_nt(Node* n, const char* name)
	{
		return n && n->sym.type == symbol_type::NONTERMINAL && n->sym.index == NT(name);
	}

	Node* child_nt(Node* n, const char* name)
	{
		if (!n) return nullptr;
		for (auto c : n->ch)
			if (is_nt(c, name)) return c;
		return nullptr;
	}

	// EXPRESSION или CONST_EXPRESSION, что найдётся
	Node* child_expr(Node* n)
	{
		Node* e = child_nt(n, "EXPRESSION");
		return e ? e : child_nt(n, "CONST_EXPRESSION");
	}

	bool is_break(Node* n)
	{
		return n->ch.empty() && n->sym.type == symbol_type::KEYWORD && text_of(n->tok) == "break";
	}

	void leaves(Node* n, std::vector<Node*>& v)
	{
		if (n->ch.empty())
		{
			if (n->sym.type != symbol_type::NONTERMINAL && n->sym.type != symbol_type::EPSILON)
				v.push_back(n);
			return;
		}
		for (auto c : n->ch) leaves(c, v);
	}

	std::string label(int l) { return "m" + std::to_string(l); }
	void emit_label(int l) { poliz.push_back(label(l) + ":"); }
	void emit_jump(const char* kind, int l) { poliz.push_back(label(l)); poliz.push_back(kind); }

	static int prio(const std::string& o)
	{
		if (o == "neg") return 4;
		if (o == "*") return 3;
		if (o == "+" || o == "-") return 2;
		return 1;                                  // == != <
	}

	// ---------- выражение: листья слева направо -> ОПЗ (алгоритм Дейкстры) ----------
	void gen_expr(Node* e)
	{
		std::vector<Node*> lv;
		leaves(e, lv);
		std::vector<std::string> st;
		bool prev_operand = false;                 // перед этим был операнд или ')'

		for (Node* t : lv)
		{
			std::string s = text_of(t->tok);
			int ty = t->sym.type;
			if (ty == symbol_type::ID || ty == symbol_type::CONST_ID || ty == symbol_type::INT_LITERAL)
			{
				poliz.push_back(s);
				prev_operand = true;
			}
			else if (s == "(")
			{
				st.push_back(s);
				prev_operand = false;
			}
			else if (s == ")")
			{
				while (st.back() != "(") { poliz.push_back(st.back()); st.pop_back(); }
				st.pop_back();
				prev_operand = true;
			}
			else
			{
				if (s == "-" && !prev_operand) s = "neg";     // унарный минус
				while (!st.empty() && st.back() != "(" &&
					(prio(st.back()) > prio(s) || (prio(st.back()) == prio(s) && s != "neg")))
				{
					poliz.push_back(st.back());
					st.pop_back();
				}
				st.push_back(s);
				prev_operand = false;
			}
		}
		while (!st.empty()) { poliz.push_back(st.back()); st.pop_back(); }
	}

	// ---------- обход дерева ----------
	// break_label: метка конца ближайшего switch (-1 вне switch)
	void gen(Node* n, int break_label)
	{
		if (is_nt(n, "ASSIGNMENT"))                // id = EXPRESSION ;
		{
			poliz.push_back(text_of(n->ch[0]->tok));
			gen_expr(child_expr(n));
			poliz.push_back("=");
		}
		else if (is_nt(n, "DECLARATION"))          // int id [= EXPRESSION] ;
		{
			Node* init = child_expr(child_nt(n, "OPTIONAL_INIT"));
			if (init)
			{
				poliz.push_back(text_of(n->ch[1]->tok));
				gen_expr(init);
				poliz.push_back("=");
			}
		}
		else if (is_nt(n, "CONST_ASSIGNMENT"))     // const int c = EXPRESSION ;
		{
			poliz.push_back(text_of(n->ch[2]->tok));
			gen_expr(child_expr(n));
			poliz.push_back("=");
		}
		else if (is_nt(n, "SWITCH"))
		{
			int end = label_cnt++;
			std::string t = "t" + std::to_string(tmp_cnt++);
			gen_expr(child_expr(n));
			poliz.push_back(t);
			poliz.push_back("=");
			gen_cases(child_nt(n, "SWITCH_BODY"), t, end);
			emit_label(end);
		}
		else if (is_break(n))
		{
			if (break_label < 0)
			{
				std::cerr << "Semantic error: break outside switch\n";
				exit(1);
			}
			emit_jump("БП", break_label);
		}
		else
		{
			for (auto c : n->ch) gen(c, break_label);   // BLOCK, LOOP_BLOCK, ...
		}
	}

	void collect_cases(Node* n, std::vector<Node*>& cases)
	{
		if (is_nt(n, "CASE") || is_nt(n, "DEFAULT")) { cases.push_back(n); return; }
		for (auto c : n->ch) collect_cases(c, cases);
	}

	void gen_cases(Node* body, const std::string& t, int end)
	{
		std::vector<Node*> cases;
		collect_cases(body, cases);

		int next = -1;                             // метка «не совпало» предыдущего case
		bool first = true;
		for (Node* c : cases)
		{
			if (is_nt(c, "CASE"))
			{
				int body_label = -1;
				if (!first)                        // проваливание мимо проверки
				{
					body_label = label_cnt++;
					emit_jump("БП", body_label);
				}
				if (next != -1) emit_label(next);

				poliz.push_back(t);
				gen_expr(child_nt(c, "CONST_EXPRESSION"));
				poliz.push_back("==");
				next = label_cnt++;
				emit_jump("УПЛ", next);

				if (body_label != -1) emit_label(body_label);
				gen(child_nt(c, "LOOP_BLOCK"), end);
				first = false;
			}
			else                                   // default
			{
				if (next != -1) { emit_label(next); next = -1; }
				gen(child_nt(c, "LOOP_BLOCK"), end);
			}
		}
		if (next != -1) emit_label(next);          // нет default: не совпало -> конец
	}

public:

	SyntaxAnalyzer()
		: parsing_table(PARSING_TABLE), static_table(STATIC_TABLE)
	{}

	void parse(std::string file_name)
	{
		std::ifstream input_stream(file_name);

		std::queue<symbol> tokens;

		int token_index = 0;
		int token_type = 0;

		while (input_stream >> token_type >> token_index)
		{
			tokens.push({ token_type, token_index });
			std::cout << "token:" << readable({ token_type, token_index }) << '\n';
		}
		tokens.push(symbol{ symbol_type::ENDOFFILE, -1 });

		// стек хранит символ и соответствующий ему узел дерева
		std::stack<std::pair<symbol, Node*>> parse_stack;

		root = make_node(symbol{ symbol_type::NONTERMINAL, static_table.find("S") });
		parse_stack.push({ symbol{ symbol_type::ENDOFFILE, -1 }, nullptr });
		parse_stack.push({ root->sym, root });

		int count = 0;

		while (!compare_symbols(parse_stack.top().first, symbol{ ENDOFFILE, -1 }))
		{
			count++;
			symbol top = parse_stack.top().first;
			Node* node = parse_stack.top().second;

			std::cout << count << " stack:\t" << readable(top) << '\t';
			std::cout << "token:" << readable(tokens.front()) << '\n';

			// терминал
			if (top.type != symbol_type::NONTERMINAL)
			{
				if (compare_symbols(top, tokens.front()))
				{
					node->tok = tokens.front();
					parse_stack.pop();
					tokens.pop();
				}
				else
				{
					std::cerr << "Expected symbol: " << readable(top) << '\n';
					exit(1);
				}
				continue;
			}

			// нетерминал
			const std::vector<symbol>* rule_ptr = find_rule(top.index, tokens.front());
			if (rule_ptr == nullptr)
			{
				std::cerr << "No usable rule for " << readable(top)
					<< " and token " << readable(tokens.front()) << '\n';
				exit(1);
			}

			parse_stack.pop();
			const std::vector<symbol>& rule = *rule_ptr;

			if (compare_symbols(rule[0], symbol{ symbol_type::EPSILON, NONSPECIFIC_INDEX }))
				continue;                          // у узла нет детей

			for (const auto& s : rule)
			{
				if (s.type == symbol_type::ACTION) // действия в дерево не попадают
					continue;
				node->ch.push_back(make_node(s));
			}
			for (int i = (int)node->ch.size() - 1; i >= 0; i--)
				parse_stack.push({ node->ch[i]->sym, node->ch[i] });
		}

		if (tokens.front().type != symbol_type::ENDOFFILE)
		{
			std::cerr << "Unexpected tokens after end of program\n";
			exit(1);
		}

		// ОПЗ
		poliz.clear();
		label_cnt = 0;
		tmp_cnt = 0;
		gen(root, -1);
		print_poliz();
	}

	void print_poliz()
	{
		std::cout << "\nPOLIZ:\n";
		for (const auto& s : poliz)
		{
			std::cout << s;
			bool end_of_line = (s == "УПЛ" || s == "БП" || s == "=" || s.back() == ':');
			std::cout << (end_of_line ? '\n' : ' ');
		}
	}

	const std::vector<std::string>& get_poliz() const { return poliz; }
};