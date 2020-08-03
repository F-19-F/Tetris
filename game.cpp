#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/ioctl.h>              //linux/unix
#include "include/terminal-linux.h" //linux/unix
#include "include/game_core.h"
#include "include/size.h"
using namespace std;
int main()
{
    Key_dec Key;
    int i;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    model *a;
    Key.start();
    game_core b(w.ws_row,(int)((w.ws_col)/3),1);
    //b.debug_core();
    //b.print();
    while (!b.over)
    {
        i=rand() % 5+1;
        a=new model(i);
        b.Add_model(a,&Key);
        delete a;
    }
    Key.stop();
    cursor_move(w.ws_row,w.ws_col);
    cout<<endl;
    return 0;
}