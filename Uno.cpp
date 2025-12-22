
#include <iostream>


int main()
{



    char cha[2][4] = {
  { 'h', 'f', 'v', 'g'},
  { 'a', 's', 'd', 'g'}
    };
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            std::cout << cha[j][i];
        }
    }
    

    return 0;
}
