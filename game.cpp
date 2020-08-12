#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#ifndef _WIN64
#include <sys/ioctl.h> //linux/unix
#include "include/terminal-POSIX.hpp" //linux/unix
#else
#include "include/terminal-windows.hpp"
#endif
#include "include/game_core.hpp"
#include "include/size.hpp"
using namespace std;
int main()
{
    Key_dec Key;
    int i;
    #ifndef _WIN64
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    #endif
    model *a;
    srand((unsigned)time(NULL));
    Key.start();
    #ifndef _WIN64
    game_core b(w.ws_row,(int)((w.ws_col)/3),5);
    #else
    game_core b(30,15,5);
    #endif
    clean_screen();
    cout.flush();
    //正常//
    b.print();
    while (!b.over)
    {
        i=rand() % 5+1;
        //i=4;
        a=new model(i);
        b.Add_model(a,&Key);
        delete a;
        Key.clean();
        //cursor_move(1,1);
        b.print();
    }
    Key.stop();
    #ifndef _WIN64
    cursor_move(w.ws_row,w.ws_col);
    end_all();
    #endif
    cout<<endl;
    cout<<"等待各线程结束";
    cout.flush();
    this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}