#include "lexical_analyzer.h"
#include "grammar_parser.h"
#include "syntax_analyzer.h"

int main()
{
	//LexicalAnalyzer lx;
	//lx.tokenize("test_program1.txt");

	GrammarParser gp;

	gp.print_nullable();
	gp.print_first();
	gp.print_follow();
	gp.print_parsing_table();

	SyntaxAnalyzer s;
	s.parse("tokens.txt");
	return 0;
}
