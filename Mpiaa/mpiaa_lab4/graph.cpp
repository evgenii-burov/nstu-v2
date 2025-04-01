#include "graph.h"

Graph::Graph(std::ifstream& input_stream)
{
    if (!input_stream)
    {
        throw std::string("Graph file couldn't be opened");
    }
    int numVertices0, numEdges0;
    input_stream >> numVertices0 >> numEdges0;
    if (numVertices0 < 1)
    {
        throw std::string("Incorrect parameter: number of vertices must be positive");
    }
    if (numEdges0 < 0 || numEdges >numVertices0 * (numVertices0 - 1) / 2)
    {
        throw std::string("Incorrect parameter: incorrect number of edges");
    }
    numEdges = numEdges0;
    numVertices = numVertices0;
    graph.resize(numVertices);
    int a, b;
    for (int i = 0; i < numEdges; i++)
    {
        input_stream >> a >> b;
        graph[a - 1].push_back(b - 1);
        graph[b - 1].push_back(a - 1);
    }
    input_stream.close();
    for (auto& adj_list : graph)
    {
        vectorBubbleSort(adj_list);
    }
}

void Graph::writeAdjacencyList(std::ofstream &output_stream) const
{
    for (int i = 0; i < numVertices; i++)
    {
        output_stream << i + 1 << ": ";
        for (const auto& vertex : graph[i])
        {
            output_stream << vertex + 1 << "\t";
        }
        output_stream << "\n";
    }
    output_stream.close();
}

void Graph::breadthFirstSearch(std::ofstream& output_stream, int start) const
{
    std::vector<bool> visited(graph.size(), false);
    std::queue<int> plan;
    plan.push(start-1);
    int currentVertex;
    while (!plan.empty())
    {
        currentVertex = plan.front();
        plan.pop();
        if (visited[currentVertex])
            continue;
        output_stream << currentVertex + 1 << "\t";
        visited[currentVertex] = true;
        for (const auto& adjVertex : graph[currentVertex]) {
            if (!visited[adjVertex])
                plan.push(adjVertex);
        }
    }
    output_stream.close();
}

void Graph::writeGraphComponentsToFile(std::string output_file_name) const
{
    std::vector<std::vector<int>> graph_components;
    std::vector<int> current_component;
    std::vector<bool> visited;
    std::queue<int> plan;
    int current_vertex;
    visited.assign(numVertices, false);
    for (int start = 0; start < numVertices; start++)
    {
        current_component.resize(0);
        if (visited[start])
            continue;
        plan.push(start);
        while (!plan.empty())
        {
            current_vertex = plan.front();
            plan.pop();
            if (visited[current_vertex])
                continue;
            current_component.push_back(current_vertex);
            visited[current_vertex] = true;
            for (const auto& adjVertex : graph[current_vertex]) {
                if (!visited[adjVertex])
                    plan.push(adjVertex);
            }
        }
        graph_components.push_back(current_component);
    }
    for (auto& component : graph_components)
    {
        vectorBubbleSort(component);
    }
    std::ofstream output_stream(output_file_name);
    if (!output_stream)
        throw std::string("Couldn't open file " + output_file_name);
    output_stream << "Graph has " << graph_components.size() << " component(s).\n";
    for (int i = 0; i < graph_components.size(); i ++)
    {
        output_stream << "Component #" << i+1 << ": ";
        for (const auto& vertex : graph_components[i])
        {
            output_stream << vertex+1 << " ";
        }
        output_stream << "\n";
    }
}

void Graph::vectorBubbleSort(std::vector<int>& v) const
{
    if (v.size() < 2)
        return;
    bool swapped = true;
    int temporary;
    for (int i = 0; swapped; i++)
    {
        swapped = false;
        for (int j = 0; j < v.size() - 1 - i; j++)
        {
            if (v[j] > v[j + 1])
            {
                temporary = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temporary;
                swapped = true;
            }
        }
    }
}