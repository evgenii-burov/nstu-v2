#pragma once
#include <set>
#include <string>
#include <fstream>

class StaticTable
{
private:
	std::set<std::string> table;
	std::string table_type;
public:
	StaticTable(std::string file_name, std::string type0) : table_type(type0)
	{
		std::string word_type;
		std::string word;
		std::ifstream input_stream(file_name);
		while (!input_stream.eof())
		{
			input_stream >> word_type >> word;
			if(word_type == table_type)
				table.insert(word);
		}
		input_stream.close();
	}
	bool contains(std::string word)
	{
		return table.find(word) != table.end();
	}
};