#include "header.h"

void priority_queue_task(int n) {
   std::ofstream fout("priority_queue.txt");
   if (!fout.is_open()) return;

   fout << n << "\n";

   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<int> number_in_queue_dist(0, n - 1);
   std::uniform_real_distribution<double> real_dist(0.0, 100.0);

   for (int i = 0; i < n; ++i) {
      fout << number_in_queue_dist(rd) << " " << real_dist(gen) << "\n";
   }
   fout.close();

   std::ifstream fin("priority_queue.txt");
   if (!fin.is_open()) return;

   int count;
   fin >> count;

   std::priority_queue<
      IntDouble,
      std::vector<IntDouble>,
      std::greater<IntDouble>
   > pq;

   for (int i = 0; i < count; ++i) {
      int int_num;
      double real_num;
      fin >> int_num >> real_num;
      pq.push(IntDouble(int_num, real_num));
   }
   fin.close();

   std::ofstream out("priority_queue_out.txt");
   if (!out.is_open()) return;

   while (!pq.empty()) {
      auto pair = pq.top();
      out << pair.first << " " << pair.second << "\n";
      pq.pop();
   }
   out.close();
}