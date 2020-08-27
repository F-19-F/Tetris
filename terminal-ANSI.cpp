#ifndef _WIN32
#include <iostream>
#include "include/ANSI_control.hpp"
#include "include/game_core.hpp"
using namespace std;
void Set_Default_color(int r, int g, int b, int f_r, int f_g, int f_b)
{
    if (!compat_mode)
    {
        cout << "\033[48;2;" << r << ";" << g << ";" << b << "m"; //设置背景色
        cout.flush();
        //clean_screen();
        cout << "\033[38;2;" << f_r << ";" << f_g << ";" << f_b << "m"; //设置前景色
        cout.flush();
    }
    else
    {
        cout << "\033[40m"; //黑色背景
        cout.flush();
        clean_screen();
        cout << "\033[33m"; //黄色前景
        cout.flush();
    }
}
void color(int i)
{
    if (!compat_mode)
    {
        static int Temp[6][3] = {Random_Color_1, Random_Color_2, Random_Color_3, Random_Color_4, Random_Color_5, Random_Color_6};
        if (i == 7)
        {
            cout << "\033[38;2;" << 158 << ";" << 173 << ";" << 134 << "m";
            return;
        }
        cout << "\033[38;2;";
        for (int j = 0; j < 3; j++)
        {
            cout << Temp[i - 1][j];
            if (j < 2)
            {
                cout << ";";
            }
        }
        cout << "m";
    }
    else
    {
        if (i == 7)
        {
            cout << "\033[33m";
            return;
        }
        cout << "\033[" << (30 + i) << "m";
    }
    cout.flush();
}
void Reset_color()
{
    end_all();
}
#endif