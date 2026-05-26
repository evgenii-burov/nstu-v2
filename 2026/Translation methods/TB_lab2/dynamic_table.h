#pragma once
#include <string>
#include <vector>
#include <stdexcept>

struct lexeme
{
	std::string name;
	// int
	std::string type;
	// const_id or int_literal
	int token_type;
	std::string value;
};

class DynamicTable
{
private:
	std::vector<lexeme> table;

public:
	int find(std::string name)
	{
		for (const auto lexem : table)
		{
			if (lexem.name == name)
				return true;
		}
		return false;
	}

	int insert(std::string name, std::string type, int token_type, std::string value)
	{
		if (find(name)) {
			return false;
		}
		try {
			std::stoi(value);
		}
		catch (std::invalid_argument) {
			value = "";
		}
		table.push_back({ name, type, token_type, value });
		return true;
	}

	void output_table(std::ostream& stream) {
		stream << "NAME\t" << "TYPE\t" << "TOK_TYPE\t" << "VALUE\n";
		for (const auto entry : table)
		{
			if (!entry.name.empty()) {
				stream << entry.name << '\t'
					<< entry.type << '\t'
					<< entry.token_type << '\t'
					<< entry.value << '\n';
			}
		}
		std::cout << '\n';
	}

	lexeme at(int index)
	{
		return table[index];
	}
};