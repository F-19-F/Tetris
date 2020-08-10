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
    game_core b(w.ws_row,(int)((w.ws_col)/3),5);
    //b.debug_core();
    clean_screen();
    cout.flush();
    /*b.debug_core();
    b.print();
    this_thread::sleep_for(std::chrono::milliseconds(500));
    b.clean();
    b.print();*/
    //正常//
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
    cout<<"等待各线程结束";
    cout.flush();
    this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}