#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;

extern "C" double __cdecl CalcFunc(double x, int* ok);

static void cout_cyrillic(const char* text)
{
    char oem[256];
    CharToOemA(text, oem);
    cout << oem;
}

int main()
{
    double x = 0.0;
    int    ok = 0;

    cout_cyrillic("Вычисление функции f(x) = (x + 2*x^2)/(x - 1)\n");
    cout_cyrillic("Введите аргумент x: ");
    cin >> x;

    if (!cin)
    {
        cout_cyrillic("Ошибка: нужно ввести вещественное число\n");
        return 1;
    }

    double result = CalcFunc(x, &ok);

    if (!ok)
    {
        cout_cyrillic("Функция не определена в точке x = 1 (деление на ноль)\n");
        return 1;
    }

    cout << setprecision(15);
    cout << "f(" << x << ") = " << result << endl;
    return 0;
}