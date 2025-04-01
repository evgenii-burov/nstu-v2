#include "graph.h"

using namespace std;

int main()
{
    try
    {
        std::ifstream input_stream;
        input_stream.open("graph.txt");
        Graph g = Graph(input_stream);
        std::ofstream output_stream;
        output_stream.open("adjacency_list.txt");
        g.writeAdjacencyList(output_stream);
        output_stream.open("vertex_list.txt");
        int starting_vertex;
        std::cout << "Input a starting vertex (graph has " << g.getVertices() << " vertices): ";
        std::cin >> starting_vertex;
        if (starting_vertex < 1 || starting_vertex > g.getVertices())
            throw std::string("Invalid starting vertex");
        g.breadthFirstSearch(output_stream, starting_vertex);
        g.writeGraphComponentsToFile("graph_components.txt");
    }
    catch (std::string error)
    {
        std::cout << error;
    }
}