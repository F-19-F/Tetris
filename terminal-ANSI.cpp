#ifndef _WIN32
#include <iostream>
#include "include/ANSI_control.hpp"
#include "include/model.hpp"
using namespace std;
int Temp[6][3]={Random_Color_1,Random_Color_2,Random_Color_3,Random_Color_4,Random_Color_5,Random_Color_6};
void Set_Default_color(int r,int g,int b,int f_r,int f_g,int f_b)
{
    cout<<"\033[48;2;"<<r<<";"<<g<<";"<<b<<"m";//设置背景色
    cout.flush();
    //clean_screen();
    cout<<"\033[38;2;"<<f_r<<";"<<f_g<<";"<<f_b<<"m";//设置前景色
    cout.flush();
}
void color(int i)
{
    if (i>7)
    {
        return;
    }
    cout<<"\033[38;2;";
    for (int j=0;j<3;j++)
    {
        cout<<Temp[i-1][j];
        if(j<2)
        {
            cout<<";";
        }
    }
    cout<<"m";
    cout.flush();
}
void Reset_color()
{
    end_all();
}
#endif