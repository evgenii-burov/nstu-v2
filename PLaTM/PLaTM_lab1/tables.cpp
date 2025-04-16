#include "tables.h"

ConstantTable::ConstantTable(std::string file_name)
{
	std::ifstream input_stream(file_name);
	if (!input_stream)
	{
		throw std::string("Couldn't open file " + file_name);
	}
	std::string key_word;
	while (!input_stream.eof())
	{
		input_stream >> key_word;
		table.insert(key_word);
	}
}

bool ConstantTable::has(std::string element)
{
	return table.find(element) != table.end();
}

int ConstantTable::find(std::string element)
{
	return std::distance(table.begin(), table.find(element));
}

std::string ConstantTable::at_index(int index)
{
	std::set<std::string>::iterator iter = table.begin();
	for (int i = 0; i<index; i++)
	{
		iter++;
	}
	return *iter;
}

void ConstantTable::print_table()
{
	for (const auto& elem : table)
	{
		std::cout << elem << "\n";
	}
	std::cout << "\n";
}