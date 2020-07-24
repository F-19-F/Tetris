#include <iostream>
using namespace std;
#include "./include/terminal-linux.h"
void getkey(int output)
{
    char key;
    system ("stty raw");
    while (1)
    {
        if ((key=getchar())=='a')
        {
            cout<<"up"<<endl<<endl<<endl<<endl;
            black_background();
            cout.flush();
        }
    }
    system ("stty cooked");
    
}