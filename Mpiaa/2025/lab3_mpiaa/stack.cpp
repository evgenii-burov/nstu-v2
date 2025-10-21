#include "header.h"


void stack_task(int n)
{
   std::ofstream fout("stack.txt");
   if (!fout.is_open()) return;

   std::random_device rd;
   std::uniform_int_distribution<int> value_dist(0, n - 1);

   fout << n << "\n";
   for (int i = 0; i < n; ++i)
   {
      fout << value_dist(rd) << " ";
   }
   fout.close();

   std::ifstream fin("stack.txt");
   if (!fin.is_open()) return;

   int count;
   fin >> count;

   std::stack<int> stack;
   for (int i = 0; i < count; ++i) {
      int num;
      fin >> num;
      stack.push(num);
   }
   fin.close();

   std::ofstream out("stack_out.txt");
   if (!out.is_open()) return;

   while (!stack.empty()) {
      out << stack.top() << " ";
      stack.pop();
   }
   out.close();
}