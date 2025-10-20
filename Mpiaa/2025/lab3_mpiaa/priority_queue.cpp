#include "header.h"

void priority_queue_task(int n) {
   // Генерация исходного файла с парами чисел
   std::ofstream fout("priority_queue.txt");
   if (!fout.is_open()) return;

   fout << n << "\n";  //количество записей

   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_real_distribution<double> real_dist(0.0, 100.0);

   for (int i = 0; i < n; ++i) {
      int int_num = i;  // Целое число (можно использовать случайное)
      double real_num = real_dist(gen);  // Вещественное число
      fout << n - int_num << " " << real_num << "\n";
   }
   fout.close();

   // Чтение данных и заполнение очереди с приоритетом
   std::ifstream fin("priority_queue.txt");
   if (!fin.is_open()) return;

   int count;
   fin >> count;  // Читаем количество элементов

   // Очередь с приоритетом, сортирующая по возрастанию целого числа
   // Используем greater для сортировки по возрастанию
   std::priority_queue<
      std::pair<int, double>,
      std::vector<std::pair<int, double>>,
      std::greater<std::pair<int, double>>
   > pq;

   for (int i = 0; i < count; ++i) {
      int int_num;
      double real_num;
      fin >> int_num >> real_num;
      pq.push(std::make_pair(int_num, real_num));
   }
   fin.close();

   // 3. Запись отсортированных пар в файл
   std::ofstream out("priority_queue_out.txt");
   if (!out.is_open()) return;

   out << count << "\n";  // Записываем количество записей

   while (!pq.empty()) {
      auto pair = pq.top();
      out << pair.first << " " << pair.second << "\n";
      pq.pop();
   }
   out.close();
}