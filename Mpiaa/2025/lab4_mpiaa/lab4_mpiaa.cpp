#include "functions.h"



int main()
{
   int n, m, start;

   // Чтение графа из файла
   vector<vector<int>> graph = readGraph("input.txt", n, m, start);

   // Поиск компоненты связности
   vector<int> component = bfsComponent(graph, start, n);

   // Запись результата
   writeResult("output.txt", component);

   cout << " \n done";
   return 0;
   
}
