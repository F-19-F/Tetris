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
    model *a;
    size ini_size;
    ini_size=Getsize();
    srand((unsigned)time(NULL));
    Key.start();
    game_core b(ini_size.r,(int)((ini_size.c)/3),5);
    //game_core b(30,15,5);
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
    cursor_move(ini_size.r,ini_size.c);
    end_all();
    cout<<endl;
    cout<<"等待各线程结束";
    cout.flush();
    this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}