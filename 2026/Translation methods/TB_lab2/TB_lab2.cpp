#include "lexical_analyzer.h"
#include "grammar_parser.h"

int main()
{
	GrammarParser gp;

	//gp.print_nullable();
	//gp.print_first();
	gp.print_follow();
}
