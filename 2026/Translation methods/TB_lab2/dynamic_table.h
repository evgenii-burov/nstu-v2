#pragma once
#include <string>
#include <vector>
#include <stdexcept>

struct lexeme
{
	std::string name;
	std::string type;
	std::string value;
};

class DynamicTable
{
private:
	std::string name;
	size_t table_size = 1000;
	size_t occupied_space = 0;
	std::vector < std::vector<lexeme> > table;

	size_t hash(std::string name) const
	{
		return std::hash<std::string>{}(name) % table_size;
	}
public:
	DynamicTable(std::string name0) : name(name0)
	{
		table.assign(table_size, {});
	}

	bool contains(std::string name)
	{
		for (const auto lexem : table[hash(name)])
		{
			if (lexem.name == name)
				return true;
		}
		return false;
	}

	bool insert(std::string name, std::string type)
	{
		if (occupied_space > (table_size / 2)) {
			resize(table_size * 2);
		}

		size_t name_hash = hash(name);
		if (contains(name)) {
			return false;
		}
		std::string value = name;
		try {
			std::stoi(value);
		}
		catch (std::invalid_argument) {
			value = "";
		}
		table[name_hash].push_back({ name, type, value });
		occupied_space++;
		return true;
	}

	void resize(size_t new_size)
	{
		table_size = new_size;
		std::vector<std::vector<lexeme>> new_table;
		new_table.assign(table_size, {});

		for (auto& bucket : table)
		{
			for (auto& lexem : bucket)
			{
				new_table[hash(lexem.name)].push_back({ lexem.name, lexem.type, lexem.value });
			}
		}
		table = std::move(new_table);
	}

	void output_table(std::ostream& stream) {
		stream << name << ":\n";
		stream << "NAME\t" << "TYPE\t" << "VALUE\n";
		for (const auto bucket : table)
		{
			for (const auto entry : bucket) {
				if (!entry.name.empty()) {
					stream << entry.name << '\t'
						<< entry.type << '\t'
						<< entry.value << '\n';
				}
			}
		}
		std::cout << '\n';
	}
};