#include "linear_system.h";

int main()
{
    try
    {
        //changed smth
        precision var;
        LinearSystem slau = LinearSystem("parameters.txt");
        slau.print();
        slau.solveBlockRelaxation(2);
    }
        catch (std::string err)
        {
            std::cerr << "\n" << err;
        }

}