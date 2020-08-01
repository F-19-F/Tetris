#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/ioctl.h>              //linux/unix
#include "include/terminal-linux.h" //linux/unix
#include "include/game_core.h"
#include "include/size.h"
#include "include/keydec.h"
using namespace std;
int main()
{
    /*Key_dec a;
    //key_base temp;
    int temp;
    a.start();
    int c=10;
    while (1)
    {
        temp = a.pop();
        if (temp!= 0)
        {
            cout << temp<< endl;
            if (temp==up)
            break;
        }
    }
    a.stop();
    this_thread::sleep_for(std::chrono::milliseconds(200));*/
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    model *a;
    game_core b(w.ws_row,(int)((w.ws_col*2)/3),1);
    for (int i=1;i<=5;i++)
    {
        ;
        a=new model(i);
        b.Add_model(a);
    }
    //this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;
}