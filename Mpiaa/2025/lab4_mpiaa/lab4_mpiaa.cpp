#include "functions.h"



int main()
{
   int n, m, start;

   // Чтение графа из файла
   vector<vector<int>> graph = readGraph("input.txt", n, m, start);

   // Поиск компоненты связности
   vector<int> component = bfsComponent(graph, start, n);

   // Запись результата
   writeResult("output_task1.txt", component);

   writeAdjacencyList("adjacency_list.txt", graph);

   vector<vector<int>> components = findGraphComponents(graph, n);

   writeComponents("output_task2.txt", components);

   //mergeGraphs("output_task3.txt");

   cout << " \n done";
   return 0;
   
}
