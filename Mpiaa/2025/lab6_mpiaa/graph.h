#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>

class Graph
{
private:
	struct edge
	{
		int vertex;
		int neighbour_vertex;
		int distance;

		edge(int vertex0, int neighbour_vertex0, int distance0)
		{
			vertex = vertex0;
			neighbour_vertex = neighbour_vertex0;
			distance = distance0;
		}

		bool operator<(const edge& other) const
		{
			return this->distance < other.distance;
		}
	};

	int n, m;
	std::vector<std::vector<edge>> graph;
	//std::vector<std::vector<std::pair<int, int>>> minimal_distances;

public:
	Graph(std::string filename);
	void dijkstra(int start_vertex);
};