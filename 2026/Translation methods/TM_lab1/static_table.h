#include <set>
#include <string>
#include <fstream>

class StaticTable
{
private:
	std::set<std::string> table;
public:
	StaticTable(std::string file_name)
	{
		std::string word;
		std::ifstream input_stream(file_name);
		while (!input_stream.eof())
		{
			std::getline(input_stream, word);
			table.insert(word);
		}
	}
	bool contains(std::string word)
	{
		return table.find(word) != table.end();
	}
};