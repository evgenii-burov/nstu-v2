#include "header.h"


void stack_task(int n)
{
   // Генерация исходного файла с числами
   std::ofstream fout("stack.txt");
   if (!fout.is_open()) return;

   fout << n << "\n";  // количество записей
   for (int i = 0; i < n; ++i)
   {
      fout << i << " ";
   }
   fout.close();

   // Чтение данных и заполнение стека
   std::ifstream fin("stack.txt");
   if (!fin.is_open()) return;

   int count;
   fin >> count;  // Читаем количество элементов

   std::stack<int> stack;
   for (int i = 0; i < count; ++i) {
      int num;
      fin >> num;
      stack.push(num);  // Помещаем элементы в стек
   }
   fin.close();

   // 3. Запись элементов в обратном порядке
   std::ofstream out("stack_out.txt");
   if (!out.is_open()) return;

   while (!stack.empty()) {
      out << stack.top() << " ";  // Извлекаем элементы из стека (обратный порядок)
      stack.pop();
   }
   out.close();
}