п»ї#include <iostream>
#include <windows.h>
using namespace std;

extern "C" int __cdecl SubstrUpper(const char* src, char* dst, int start, int len);

static void cout_cyrillic(const char* text)
{
    char oem[256];
    CharToOemA(text, oem);
    cout << oem;
}

int main()
{
    char src[256];
    char dst[256] = { 0 };
    int  start = 0, len = 0;

    cout_cyrillic("Р’РІРµРґРёС‚Рµ СЃС‚СЂРѕРєСѓ: ");
    cin.getline(src, sizeof(src));

    cout_cyrillic("Р’РІРµРґРёС‚Рµ РЅРѕРјРµСЂ РїРѕР·РёС†РёРё: ");
    cin >> start;
    cout_cyrillic("Р’РІРµРґРёС‚Рµ РґР»РёРЅСѓ РїРѕРґСЃС‚СЂРѕРєРё: ");
    cin >> len;

    if (!cin)
    {
        cout_cyrillic("РћС€РёР±РєР°: РЅСѓР¶РЅРѕ РІРІРѕРґРёС‚СЊ С†РµР»С‹Рµ С‡РёСЃР»Р°\n");
        return 1;
    }

    if (SubstrUpper(src, dst, start, len) != 0)
    {
        cout_cyrillic("РћС€РёР±РєР°: РїРѕРґСЃС‚СЂРѕРєР° РІС‹С…РѕРґРёС‚ Р·Р° РіСЂР°РЅРёС†С‹ СЃС‚СЂРѕРєРё\n");
        return 1;
    }

    cout_cyrillic("РџРѕРґСЃС‚СЂРѕРєР°: ");
    cout << dst << endl;
    return 0;
}