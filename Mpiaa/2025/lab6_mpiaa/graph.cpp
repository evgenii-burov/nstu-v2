#include "graph.h"

Graph::Graph(std::string filename)
{
    std::ifstream input_stream(filename);
    input_stream >> n >> m;

    graph.resize(n);

    for (int i = 0; i < m; ++i) {
        int vertex, neighbour_vertex, distance;
        input_stream >> vertex >> neighbour_vertex >> distance;
        graph[vertex - 1].push_back({ distance, neighbour_vertex - 1 });
        graph[neighbour_vertex - 1].push_back({ distance, vertex - 1 });
    }

    input_stream.close();
}

void Graph::dijkstra(int origin_vertex)
{
    std::priority_queue<
        std::pair<int, int>,
        std::vector<std::pair<int, int>>,
        std::greater<std::pair<int, int>>
    > queue;

    std::vector<int> distance_to_origin(n, 2000);
    distance_to_origin[origin_vertex] = 0;
    queue.push({0, origin_vertex});

    for (const std::pair<int, int>& p : graph[origin_vertex])
    {
        queue.push(p);
    }

    while (!queue.empty())
    {
        std::pair<int, int> top = queue.top();
        queue.pop();
        int distance = top.first;
        int vertex = top.second;

        if (distance > distance_to_origin[vertex])
        {
            continue;
        }

        for (const std::pair<int, int>& p : graph[vertex])
        {
            int neighbour_distance = p.first;
            int neighbour_vertex = p.second;

            if (distance_to_origin[vertex] + neighbour_distance < distance_to_origin[neighbour_vertex])
            {
                distance_to_origin[neighbour_vertex] = distance_to_origin[vertex] + neighbour_distance;
                queue.push({ distance_to_origin[neighbour_vertex], neighbour_vertex });
            }
        }
        for (int i = 0; i < n; i++)
        {
            std::cout << distance_to_origin[i] << "\t";
        }
        std::cout << "\n---\n";
    }

    
}