#include <iostream>
#include "static_table.h"
#include "dynamic_table.h"

int main()
{
	//StaticTable st("key_words.txt");
	//std::cout << st.contains("main");
	//std::cout << st.contains("int");
	//st.contains("a");
	//st.contains("int");

	DynamicTable dt;
	std::cout << '\n' << dt.insert("vova");
	std::cout << '\n' << dt.insert("vova");
	std::cout << '\n' << dt.contains("misha");
	std::cout << '\n' << dt.contains("vova");
	std::cout << '\n' << dt.change_value("misha", 64);
	std::cout << '\n' << dt.read_value("vova");
}
