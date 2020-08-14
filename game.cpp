#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#ifndef _WIN32
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
    model *temp;
    size ini_size;
    ini_size=Getsize();
    srand((unsigned)time(NULL));
    Key.start();
    game_core b(ini_size.r-3,(int)((ini_size.c)/5),1,0,9);
    clean_screen();
    cout.flush();
    //正常//
    b.print();
    i=rand() % 5+1;
    while (!b.over)
    {
        cursor_move(ini_size.c/5+3,1);
        cout<<"分数:"<<b.get_score();
        cout.flush();
        //i=4;
        a=new model(i);
        i=rand() % 5+1;
        temp=new model(i);
        cursor_move(ini_size.c/5+3,10);
        cout<<"下一个是:";
        cursor_move(ini_size.c/5+3,11);
        temp->print_model(false);
        b.Add_model(a,&Key);
        cursor_move(ini_size.c/5+3,11);
        temp->print_model(true);
        delete a;
        delete temp;
        Key.clean();
        //cursor_move(1,1);
        b.print();
    }
    Key.stop();
    cursor_move(ini_size.r,ini_size.c);
    end_all();
    cout<<endl;
    cout<<"最终分数"<<b.get_score()<<endl;
    cout.flush();
    cout<<"游戏已结束，按任意操作键退出游戏";
    cout.flush();
    while (1)
    {
        if (Key.pop()!=0)
        {
            break;
        }
        this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return 0;
}