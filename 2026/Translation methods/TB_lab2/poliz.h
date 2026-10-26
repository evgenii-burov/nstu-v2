#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>

enum op_t { OP_ADD, OP_SUB, OP_MUL, OP_EQ, OP_NE, OP_LT, OP_NEG, OP_ASSIGN };
const int MARKER = -1;

enum poliz_kind {
    P_VAR, P_ADDR, P_CONST, P_OP,
    P_LABEL_REF, P_LABEL_DEF, P_JMP, P_JZ,
    P_STORE_TMP, P_TMP
};

struct poliz_item { poliz_kind kind; int value; };

struct sw_ctx {
    int tmp;
    int end_label;
    int next_label = -1;
    bool has_case = false;
    std::set<int> values;
};

// ---------- состояние ----------
inline std::vector<poliz_item> poliz;
inline std::vector<poliz_item> const_buf;
inline std::vector<poliz_item>* out = &poliz;

inline std::vector<int> opst;
inline std::vector<sw_ctx> sw_stack;
inline int tmp_count = 0;
inline int label_cnt = 0;

inline std::set<int> declared_vars;
inline std::map<int, int> consts;        // индекс const_id -> значение
inline int last_declared = -1;
inline int pending_const = -1;
inline int const_value = 0;
inline std::map<int, op_t> OP_BY_INDEX;

// ---------- вспомогательные ----------
[[noreturn]] inline void fail(const std::string& msg)
{
    std::cerr << "Semantic error: " << msg << '\n';
    exit(1);
}

inline void init_ops()                    // вызвать один раз после загрузки static_table
{
    OP_BY_INDEX[static_table.find("+")] = OP_ADD;   // ADAPT
    OP_BY_INDEX[static_table.find("-")] = OP_SUB;
    OP_BY_INDEX[static_table.find("*")] = OP_MUL;
    OP_BY_INDEX[static_table.find("==")] = OP_EQ;
    OP_BY_INDEX[static_table.find("!=")] = OP_NE;
    OP_BY_INDEX[static_table.find("<")] = OP_LT;
}

inline op_t op_code(const symbol& tok)
{
    auto it = OP_BY_INDEX.find(tok.index);
    if (it == OP_BY_INDEX.end()) fail("unknown operator");
    return it->second;
}

inline int prio(int op)
{
    switch (op)
    {
    case OP_MUL: return 3;
    case OP_ADD: case OP_SUB: return 2;
    case OP_EQ: case OP_NE: case OP_LT: return 1;
    }
    return 0;
}

inline int literal_value(int index) { return literal_table[index]; }   // ADAPT

inline void emit(poliz_kind k, int v) { out->push_back({ k, v }); }
inline int  new_label() { return label_cnt++; }
inline void emit_label(int l) { poliz.push_back({ P_LABEL_DEF, l }); }
inline void emit_jump(poliz_kind jump, int l)
{
    poliz.push_back({ P_LABEL_REF, l });
    poliz.push_back({ jump, 0 });
}

inline int eval_const(const std::vector<poliz_item>& buf)
{
    std::vector<int> st;
    for (auto& it : buf)
    {
        if (it.kind == P_CONST) { st.push_back(it.value); continue; }
        if (it.kind != P_OP) fail("non-constant expression");
        if (it.value == OP_NEG)
        {
            if (st.empty()) fail("bad constant expression");
            st.back() = -st.back();
            continue;
        }
        if (st.size() < 2) fail("bad constant expression");
        int b = st.back(); st.pop_back();
        int a = st.back(); st.pop_back();
        switch (it.value)
        {
        case OP_ADD: st.push_back(a + b);  break;
        case OP_SUB: st.push_back(a - b);  break;
        case OP_MUL: st.push_back(a * b);  break;
        case OP_EQ:  st.push_back(a == b); break;
        case OP_NE:  st.push_back(a != b); break;
        case OP_LT:  st.push_back(a < b);  break;
        default: fail("bad operator in constant expression");
        }
    }
    if (st.size() != 1) fail("bad constant expression");
    return st[0];
}

// ---------- действия ----------
inline void do_action(int a, const symbol& tok)
{
    switch (a)
    {
    case A_DECLARE:
        if (declared_vars.count(tok.index)) fail("variable redeclared");
        declared_vars.insert(tok.index);
        last_declared = tok.index;
        break;
    case A_INIT_ADDR:
        emit(P_ADDR, last_declared);
        break;
    case A_DECL_CONST:
        if (consts.count(tok.index)) fail("constant redeclared");
        pending_const = tok.index;
        break;
    case A_CONST_BEGIN:
        const_buf.clear();
        out = &const_buf;
        break;
    case A_CONST_END:
        const_value = eval_const(const_buf);
        out = &poliz;
        break;
    case A_SET_CONST:
        consts[pending_const] = const_value;
        break;

    case A_PUSH_ADDR:
        if (!declared_vars.count(tok.index)) fail("variable not declared");
        emit(P_ADDR, tok.index);
        break;
    case A_PUSH_VAR:
        if (!declared_vars.count(tok.index)) fail("variable not declared");
        emit(P_VAR, tok.index);
        break;
    case A_PUSH_CONST:
        if (tok.type == INT_LITERAL)
            emit(P_CONST, literal_value(tok.index));
        else
        {
            auto it = consts.find(tok.index);
            if (it == consts.end()) fail("constant not defined");
            emit(P_CONST, it->second);
        }
        break;

    case A_EXPR_OPEN:
        opst.push_back(MARKER);
        break;
    case A_OP: {
        int op = op_code(tok);
        while (opst.back() != MARKER && prio(opst.back()) >= prio(op))
        {
            emit(P_OP, opst.back());
            opst.pop_back();
        }
        opst.push_back(op);
        break;
    }
    case A_NEG:
        emit(P_OP, OP_NEG);
        break;
    case A_EXPR_CLOSE:
        while (opst.back() != MARKER)
        {
            emit(P_OP, opst.back());
            opst.pop_back();
        }
        opst.pop_back();
        break;
    case A_ASSIGN:
        emit(P_OP, OP_ASSIGN);
        break;

    case A_SW_BEGIN: {
        sw_ctx c;
        c.tmp = tmp_count++;
        c.end_label = new_label();
        poliz.push_back({ P_STORE_TMP, c.tmp });
        sw_stack.push_back(c);
        break;
    }
    case A_CASE_TEST: {
        sw_ctx& c = sw_stack.back();
        if (!c.values.insert(const_value).second) fail("duplicate case value");

        int body = new_label();
        if (c.has_case) emit_jump(P_JMP, body);
        if (c.next_label != -1) emit_label(c.next_label);

        poliz.push_back({ P_TMP, c.tmp });
        poliz.push_back({ P_CONST, const_value });
        poliz.push_back({ P_OP, OP_EQ });
        c.next_label = new_label();
        emit_jump(P_JZ, c.next_label);

        if (c.has_case) emit_label(body);
        c.has_case = true;
        break;
    }
    case A_DEFAULT: {
        sw_ctx& c = sw_stack.back();
        if (c.next_label != -1) { emit_label(c.next_label); c.next_label = -1; }
        break;
    }
    case A_BRK:
        if (sw_stack.empty()) fail("break outside switch");
        emit_jump(P_JMP, sw_stack.back().end_label);
        break;
    case A_SW_END: {
        sw_ctx& c = sw_stack.back();
        if (c.next_label != -1) emit_label(c.next_label);
        emit_label(c.end_label);
        sw_stack.pop_back();
        break;
    }
    }
}

// ---------- печать ----------
inline void print_poliz(std::string(*name_of_var)(int))   // ADAPT: имя переменной по индексу
{
    static const char* OPS[] = { "+", "-", "*", "==", "!=", "<", "neg", "=" };
    for (auto& it : poliz)
    {
        switch (it.kind)
        {
        case P_VAR:
        case P_ADDR:      std::cout << name_of_var(it.value) << ' ';  break;
        case P_CONST:     std::cout << it.value << ' ';               break;
        case P_OP:        std::cout << OPS[it.value] << ' ';          break;
        case P_TMP:       std::cout << 't' << it.value << ' ';        break;
        case P_STORE_TMP: std::cout << 't' << it.value << " =\n";     break;
        case P_LABEL_REF: std::cout << 'm' << it.value << ' ';        break;
        case P_JZ:        std::cout << "УПЛ\n";                       break;
        case P_JMP:       std::cout << "БП\n";                        break;
        case P_LABEL_DEF: std::cout << 'm' << it.value << ":\n";      break;
        }
    }
}