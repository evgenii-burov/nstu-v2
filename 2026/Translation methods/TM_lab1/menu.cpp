#include <iostream>
#include <string>
#include <fstream>
#include "static_table.h"
#include "dynamic_table.h"
#include "menu.h"

void main_menu()
{
	std::string user_input;

	std::cout << "Enter:\n";
	std::cout << "<1> To test the static table\n";
	std::cout << "<2> To test the dynamic table\n";
	std::cout << "<3> To exit\n>";
	while (true)
	{
		getline(std::cin, user_input);
		switch (std::stoi(user_input))
		{
		case 1:
			static_table_menu();
			break;
		case 2:
			dynamic_table_menu();
			break;
		case 3:
			std::cout << "\nExiting..";
			exit(0);
		default:
			std::cout << "\nInvalid command\n";
			break;
		}
	}


}

void static_table_menu()
{
	std::string user_input;
	int user_choice = 0;

	std::cout << "\nEnter a file name to read the key words from (key_words.txt expected): ";
	std::getline(std::cin, user_input);
	std::ifstream input_stream(user_input);
	if (!input_stream.is_open())
	{
		std::cerr << "Unable to open that file\n";
		exit(0);
	}

	StaticTable table(user_input);

	std::cout << "\nExpected word list from " << user_input << ":\n";
	while (!input_stream.eof())
	{
		std::string word;
		std::getline(input_stream, word);
		std::cout << word << "\n";
	}

	std::cout << "Enter a word to check if it is contained in the table (type /exit to exit the program):\n";

	while (true)
	{
		std::getline(std::cin, user_input);
		if (user_input == "/exit")
		{
			std::cout << "\nExiting..";
			exit(0);
		}
		if (table.contains(user_input))
		{
			std::cout << "\n" << user_input << " is in the table\n";
		}
		else
		{
			std::cout << "\n" << user_input << " is not in the table\n";
		}
	}
}

void dynamic_table_menu()
{
	DynamicTable table;

	std::string user_input;

	std::cout << "\nEnter:\n";
	std::cout << "<1> To insert an element\n";
	std::cout << "<2> To check if an element is present in the table\n";
	std::cout << "<3> To change an elements value\n";
	std::cout << "<4> To read an elements value\n";
	std::cout << "<5> To resize the table\n";
	std::cout << "<6> To exit\n";
	while (true)
	{
		std::cout << "\n>";
		getline(std::cin, user_input);
		switch (std::stoi(user_input))
		{
		case 1:
		{
			std::string value_string;
			int new_value;
			std::cout << "\nEnter the name of the element to be inserted: ";
			getline(std::cin, user_input);
			std::cout << "Enter the new value of the element: ";
			getline(std::cin, value_string);
			try
			{
				new_value = std::stoi(value_string);
			}
			catch (std::invalid_argument)
			{
				std::cerr << "Value must be an integer\n";
				break;
			}
			if (table.insert(user_input, new_value))
				std::cout << "Element inserted\n";
			else
				std::cout << "That element is already in the table\n";
			break;
		}
		case 2:
			std::cout << "\nEnter the name of the element to be checked for: ";
			getline(std::cin, user_input);
			if (table.contains(user_input))
				std::cout << "Element is in the table\n";
			else
				std::cout << "Element is not in the table\n";
			break;
		case 3:
		{
			std::string value_string;
			int new_value;
			std::cout << "\nEnter the name of the element to be changed: ";
			getline(std::cin, user_input);
			std::cout << "Enter the new value of the element: ";
			getline(std::cin, value_string);
			try
			{
				new_value = std::stoi(value_string);
			}
			catch (std::invalid_argument)
			{
				std::cerr << "Value must be an integer\n";
				break;
			}
			if (table.change_value(user_input, new_value))
				std::cout << "Elements value is changed\n";
			else
				std::cout << "There is no such element in the table\n";
			break;
		}
		case 4:
			std::cout << "\nEnter the name of the element to be read: ";
			getline(std::cin, user_input);
			if (table.contains(user_input))
				std::cout << "Value of the element is " << table.read_value(user_input) << '\n';
			else
				std::cout << "Element is not in the table\n";
			break;
		case 5:
		{
			std::cout << "\nEnter the new size of the table: ";
			getline(std::cin, user_input);
			int new_size = std::stoi(user_input);
			if (new_size > 0)
			{
				table.resize(new_size);
				std::cout << "Table has been resized\n";
			}
			else
			{
				std::cout << "Incorrect size value, re-enter the command to attempt a new resize\n";
			}
			break;
		}
		case 6:
			std::cout << "\nExiting..";
			exit(0);
		default:
			std::cout << "\nInvalid command\n";
			break;
		}
	}
}