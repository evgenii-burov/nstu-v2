#include "graph.h"

Graph::Graph(std::string filename)
{
    std::ifstream input_stream(filename);
    input_stream >> n >> m;

    graph.resize(n);

    for (int i = 0; i < m; ++i) {
        int vertex, neighbour_vertex, distance;
        input_stream >> vertex >> neighbour_vertex >> distance;
        graph[vertex - 1].push_back(edge(vertex-1, neighbour_vertex-1, distance));
        graph[neighbour_vertex - 1].push_back(edge(neighbour_vertex-1, vertex-1, distance));
    }

    input_stream.close();
}

void Graph::dijkstra(int start_vertex)
{
    std::vector<int> distances(n, 0);
    std::vector<int> previous(n, -1);
    std::priority_queue<edge, std::vector<edge>, std::less<edge>> queue;

    distances[start_vertex] = 0;
    for (edge e : graph[start_vertex])
        queue.push(e);

    while (!queue.empty()) {
        edge current = queue.top();
        queue.pop();
        int neighbour_vertex = current.neighbour_vertex;

        if (distances[neighbour_vertex] != 0)
            continue;

        distances[neighbour_vertex] = current.distance;
        previous[neighbour_vertex] = current.vertex;

        for (edge e : graph[neighbour_vertex])
            queue.push(edge(e.vertex, e.neighbour_vertex, e.distance + current.distance));
    }
    for (int i = 0; i < n; i++)
    {
        std::cout << i << ": " << distances[i] << "\n";
    }
}