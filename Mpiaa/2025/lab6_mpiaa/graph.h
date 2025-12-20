#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>

class Graph
{
private:

	int n, m;
	// pair<distance, vertex>
	std::vector<std::vector<std::pair<int, int>>> graph;
	//std::vector<std::vector<std::pair<int, int>>> minimal_distances;

public:
	Graph(std::string filename);
	void dijkstra(int start_vertex);
};