#include "header.h"

std::string input_file_name = "dynamic_array.txt";
std::string output_file_name = "dynamic_array_out.txt";

void dynamic_array_task(int n, int value_range)
{
	std::random_device rd;
	std::uniform_int_distribution<int> array_number_dist(0, n-1);
	std::uniform_int_distribution<int> value_dist(1, value_range);

	std::ofstream output_stream(input_file_name);
	output_stream << n << "\n";
	for (int i = 0; i < n; i++)
	{
		output_stream << array_number_dist(rd) << "\t" << value_dist(rd) << "\n";
	}
	output_stream.close();

	std::ifstream input_stream(input_file_name);
	int number_of_entries = 0;
	input_stream >> number_of_entries;

	std::vector<std::vector<int>> array;
	int array_number, value;

	for (int i = 0; i < number_of_entries; i++)
	{
		input_stream >> array_number >> value;

		if (array_number > array.size())
		{
			array.resize(array_number + 1);
		}

		array[array_number].push_back(value);
	}
	input_stream.close();

	output_stream = std::ofstream(output_file_name);

	for (int i = 0; i < array.size(); i++)
	{
		std::sort(array[i].begin(), array[i].end());
		array[i].erase(std::unique(array[i].begin(), array[i].end()), array[i].end());
		if (array[i].size() != 0)
		{
			output_stream << "array[" << i << "]:\t";
			for (int j = 0; j < array[i].size(); j++)
			{
				output_stream << array[i][j] << "\t";
			}
			output_stream << "\n";
		}
	}
	output_stream.close();
}