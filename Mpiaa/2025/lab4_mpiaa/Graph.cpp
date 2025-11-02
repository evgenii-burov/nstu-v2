#include"functions.h"



vector<vector<int>> readGraph(const string& filename, int& n, int& m, int& start) 
{
   ifstream fin(filename);
   fin >> n >> m;

   vector<vector<int>> graph(n + 1);

   for (int i = 0; i < m; ++i) {
      int u, v;
      fin >> u >> v;
      graph[u].push_back(v);
      graph[v].push_back(u);
   }

   fin >> start;
   fin.close();

   return graph;
}

vector<int> bfsComponent(const vector<vector<int>>& graph, int start, int n)
{
   vector<bool> visited(n + 1, false);
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
      fout << vertex << " ";
   }
   fout.close();
}
