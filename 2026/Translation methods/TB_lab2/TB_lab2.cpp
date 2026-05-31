#include "lexical_analyzer.h"
#include "grammar_parser.h"

int main()
{
	StaticTable table("static_characters.txt");
	GrammarParser gp;
	std::map<int, std::vector<std::vector<symbol>>> grammar;
	grammar = gp.parse("grammar_for_parsing.txt");
	for (const auto& elem : grammar)
	{
		std::cout << elem.first << '\n';
		for (const auto& rule : elem.second)
		{
			for (const auto& s : rule)
			{
				std::cout << table.at(s.index).second << ' ' << s.index << ", \t";
			}
			std::cout << "|\n";
		}
		std::cout << "sectionend\n";
	}
	std::cout << grammar.size();

}
