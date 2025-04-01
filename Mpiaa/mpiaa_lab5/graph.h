#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <string>

class Graph
{
private:
	int numEdges, numVertices;
	std::vector<std::vector<int>> graph;

	void vectorBubbleSort(std::vector<int>& v) const;
public:
	Graph(std::ifstream& input_stream);

	int getVertices() const
	{
		return numVertices;
	}

	void writeAdjacencyList(std::ofstream& output_stream) const;

	//breadth first graph traversal, visited vertices are written to a file in order of traversal, start vertex is
	//given in 1..num_vertices notation
	void breadthFirstSearch(std::ofstream& output_stream, int start) const;

	void writeGraphComponentsToFile(std::string output_file_name) const;
};