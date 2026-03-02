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
	std::vector<lexeme> table;
public:
	bool contains(std::string name)
	{ 
		for (auto it = table.begin(); it != table.end(); it++)
		{
			if (it->name == name)
			{
				return true;
			}
		}
		return false;
	}

	bool change_value(std::string name, int value)
	{
		for (auto it = table.begin(); it != table.end(); it++)
		{
			if (it->name == name)
			{
				it->value = value;
				return true;
			}
		}
		return false;
	}

	bool insert(std::string name)
	{
		if (!contains(name))
		{
			table.push_back(lexeme{ name, 0 });
			return true;
		}
		return false;
	}

	int read_value(std::string name)
	{
		for (auto it = table.begin(); it != table.end(); it++)
		{
			if (it->name == name)
			{
				return it->value;
			}
		}
		return -1;
	}
};