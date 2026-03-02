#include <string>
#include <vector>

struct lexeme
{
	std::string name;
	int value;
};

class DynamicTable
{
private:
	size_t table_size = 1000;
	std::vector < std::vector<lexeme> > table;

	size_t hash(std::string name) const
	{
		return std::hash<std::string>{}(name) % table_size;
	}
public:
	DynamicTable()
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

	bool change_value(std::string name, int value)
	{
		for (auto &lexem : table[hash(name)])
		{
			if (lexem.name == name)
			{
				lexem.value = value;
				return true;
			}
		}
		return false;
	}

	bool insert(std::string name)
	{
		size_t name_hash = hash(name);
		for (auto& lexem : table[name_hash])
		{
			if (lexem.name == name)
			{
				return false;
			}
		}
		table[name_hash].push_back({ name, 0 });
		return true;
	}

	int read_value(std::string name)
	{
		for (auto& lexem : table[hash(name)])
		{
			if (lexem.name == name)
			{
				return lexem.value;
			}
		}
		return 0;
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
				new_table[hash(lexem.name)].push_back({ lexem.name, lexem.value});
			}
		}
		table = std::move(new_table);
	}
};