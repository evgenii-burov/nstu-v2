#include "lexical_analyzer.h"

int main()
{
	LexicalAnalyzer lexer(std::string("static_characters.txt"));
	lexer.tokenize(std::string("test_program1.txt"));
}
