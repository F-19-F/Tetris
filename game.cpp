#include <iostream>
#include <thread>
#include <mutex>
#ifndef _WIN32
#include "include/ANSI_control.hpp" //linux/unix
#else
#include "include/WinAPI_control.hpp"
#endif
#include "include/game_core.hpp"
#include "include/size.hpp"
using namespace std;
int First_flag=0;
void Infor_print(int x,int y,game_core *core,model *next_model)
{
    
    static model Last_model=*next_model;
    if (First_flag!=0)
    {
        cursor_move(x,y+7);
        Last_model.print_model(true);
    }
    color(7);
    cursor_move(x,y);
    cout<<"Score:"<<core->get_score();
    cout.flush();
    cursor_move(x,y+3);
    cout<<"Level:"<<core->get_speed();
    cout.flush();
    cursor_move(x,y+6);
    cout<<"Next:";
    cout.flush();
    cursor_move(x,y+7);
    next_model->print_model(false);
    Last_model=*next_model;
    First_flag=1;
}
int Startgame(Key_dec *Key)
{
    int i;
    model *a;
    model *temp;
    size ini_size;
    ini_size=Getsize();
    srand((unsigned)time(NULL));
    game_core b(ini_size.r-3,(int)((ini_size.c)/5),1,0,10);
    clean_screen();
    cout.flush();
    b.print();
    i=rand() % 5+1;
    while (!b.over)
    {
        a=new model(i);
        i=rand() % 5+1;
        temp=new model(i);
        Infor_print(ini_size.c/5+5,3,&b,temp);
        b.Add_model(a,Key);
        delete a;
        delete temp;
        Key->clean();
        b.print();
    }
    cursor_move(ini_size.r,ini_size.c);
    Reset_color();
    cout<<endl;
    cout<<"最终分数"<<b.get_score()<<endl;
    cout.flush();
    First_flag=0;
    return b.get_score();
}
int main()
{
    Key_dec Key;
    Set_Default_color(BF_Default_Color_RGB);
    Key.start();
    Startgame(&Key);
    Key.stop();
    cout<<"游戏已结束，按任意键退出游戏";
    cout.flush();
    while (1)
    {
        if (Key.pop()!=0)
        {
            break;
        }
        this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    //运行完后恢复光标显示
    dishide_cusor();
    return 0;
}