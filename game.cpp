#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/ioctl.h>              //linux/unix
#include "include/terminal-linux.hpp" //linux/unix
#include "include/game_core.hpp"
#include "include/size.hpp"
using namespace std;
int main()
{
    Key_dec Key;
    int i;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    model *a;
    srand((unsigned)time(NULL));
    Key.start();
    game_core b(w.ws_row,(int)((w.ws_col)/3),1);
    //b.debug_core();
    clean_screen();
    cout.flush();
    b.print();
    while (!b.over)
    {
        i=rand() % 5+1;
        a=new model(i);
        b.Add_model(a,&Key);
        delete a;
        b.print();
    }
    Key.stop();
    cursor_move(w.ws_row,w.ws_col);
    end_all();
    cout<<endl;
    return 0;
}