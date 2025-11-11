#include"functions.h"



vector<vector<int>> readGraph(const string& filename, int& n, int& m, int& start) 
{
   ifstream fin(filename);
   fin >> n >> m;

   vector<vector<int>> graph(n);

   for (int i = 0; i < m; ++i) {
      int u, v;
      fin >> u >> v;
      graph[u-1].push_back(v-1);
      graph[v-1].push_back(u-1);
   }

   fin >> start;
   start--;
   fin.close();

   return graph;
}

vector<int> bfsComponent(const vector<vector<int>>& graph, int start, int n)
{
   vector<bool> visited(n, false);
   queue<int> q;
   vector<int> component;

   visited[start] = true;
   q.push(start);

   while (!q.empty()) {
      int current = q.front();
      q.pop();
      component.push_back(current);

      for (int neighbor : graph[current]) {
         if (!visited[neighbor]) {
            visited[neighbor] = true;
            q.push(neighbor);
         }
      }
   }

   return component;
}

void writeResult(const string& filename, const vector<int>& component)
{
   ofstream fout(filename);
   for (int vertex : component) {
      fout << vertex+1 << " ";
   }
   fout.close();
}

// Нахождение всех компонент связности через BFS
vector<vector<int>> findGraphComponents(const vector<vector<int>>& graph, int n)
{
    vector<bool> visited(n, false);
    vector<vector<int>> components;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            queue<int> q;
            vector<int> component;

            visited[i] = true;
            q.push(i);

            while (!q.empty()) {
                int current = q.front();
                q.pop();
                component.push_back(current);


                for (int neighbor : graph[current]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }

            // Сортируем вершины компоненты и добавляем в результат
            sort(component.begin(), component.end());
            components.push_back(component);
        }
    }

    return components;
}

// Запись компонент связности в файл
void writeComponents(const string& filename, const vector<vector<int>>& components)
{
    ofstream fout(filename);

    // Первая строка - количество компонент связности
    fout << components.size() << endl;

    // Затем перечисляем вершины каждой компоненты
    for (const auto& component : components) {
        for (size_t i = 0; i < component.size(); i++) {
            fout << component[i] + 1;
            if (i < component.size() - 1) {
                fout << " ";
            }
        }
        fout << endl;
    }

    fout.close();
}

void writeAdjacencyList(const string& filename, const vector<vector<int>>& graph)
{
    ofstream output_stream(filename);

    for (int i = 0; i < graph.size(); i++)
    {
        output_stream << i + 1 << ": ";
        for (int j = 0; j < graph[i].size(); j++)
        {
            output_stream << graph[i][j] + 1 << " ";
        }
        output_stream << endl;
    }

    output_stream.close();
}

void mergeGraphs(const string& filename)
{
    vector<vector<int>> graph;

    fstream input_stream("n.txt");
    int number_of_files;
    input_stream >> number_of_files;
    input_stream.close();

    for (int i = 0; i < number_of_files; i++)
    {
        input_stream = fstream(to_string(i + 1) + ".txt");
        int number_of_vertices = 0;
        input_stream >> number_of_vertices;

        vector<int> complete_graph_vertices;

        for (int j = 0; j < number_of_vertices; j++)
        {
            int vertex = 0;
            input_stream >> vertex;
            complete_graph_vertices.push_back(vertex - 1);
            bool vertex_adjacency_list_exists = false;
            for (auto& vertex_adjacency_list : graph)
            {
                if (vertex_adjacency_list[0] == vertex - 1)
                    vertex_adjacency_list_exists = true;
            }
            if(vertex_adjacency_list_exists == false)
                graph.push_back(vector<int>({ vertex - 1 }));
        }

        for (int current_vertex = 0; current_vertex < number_of_vertices - 1; current_vertex++)
        {
            for (auto& vertex_adjacency_list : graph)
            {
                if (vertex_adjacency_list[0] == complete_graph_vertices[current_vertex])
                {
                    for (int other_vertex = current_vertex + 1; other_vertex < number_of_vertices; other_vertex++)
                    {
                        vertex_adjacency_list.push_back(complete_graph_vertices[other_vertex]);
                    }
                }
            }
        }
        input_stream.close();
    }
    for (auto& vertex_adjacency_list : graph)
    {
        for (int i = 0; i < vertex_adjacency_list.size(); i++)
        {
            cout << vertex_adjacency_list[i] << "\t";
        }
        cout << endl;
    }
}