#include <iostream>
#include <cmath>
#ifndef _WIN32
#include "include/ANSI_control.hpp"
#else
#include "include/WinAPI_control.hpp"
#endif
#include "include/Model.hpp"
#include "memory.h"
using namespace std;
//方块模型,mode表示方块类型，1-5将构建预设类型，0代表自主构建
model::model(int mode)
{
    memset(base, 0, 64 * sizeof(bool));
    memset(temp, 0, 16 * sizeof(bool));
    switch (mode)
    {
    case 1:
        base[3][0] = true; //  ####
        base[3][2] = true; //  ########
        base[3][4] = true; //  ########
        base[5][2] = true; //  ####
        center_x = 3;
        center_y = 2;
        Color = 1;
        break;
    case 2:
        base[1][2] = true; //  ######
        base[3][2] = true; //  ######
        base[3][4] = true; //      ######
        base[5][4] = true; //      ######
        center_x = 3;
        center_y = 3;
        Color = 2;
        break;
    case 3:
        base[0][0] = true; //  ########
        base[2][0] = true; //  ########
        base[0][2] = true; //  ########
        base[2][2] = true; //  ########
        center_x = 1;
        center_y = 1;
        Color = 3;
        break;
    case 4:
        base[3][0] = true; //  ##
        base[3][2] = true; //  ##
        base[3][4] = true; //  ##
        base[3][6] = true; //  ##
        center_x = 3;
        center_y = 3;
        Color = 4;
        break;
    case 5:
        base[1][3] = true; //          ####
        base[3][3] = true; //          ####
        base[5][3] = true; //  ############
        base[5][1] = true; //  ############
        center_x = 3;
        center_y = 3;
        Color = 5;
        break;
    case 6:
        base[1][3] = true; //  ####
        base[3][3] = true; //  ####
        base[5][3] = true; //  ############
        base[1][1] = true; //  ############
        center_x = 3;
        center_y = 3;
        Color = 5;
        break;
    case 7:
        base[5][2] = true; //     #######
        base[3][2] = true; //     #######
        base[3][4] = true; // ######
        base[1][4] = true; // ######
        center_x = 3;
        center_y = 3;
        Color = 2;
        break;
    default:
        break;
    }
    get_temp();
}
//输出原始数据，以便调试--for debug
void model::print_row(int mode)
{
    if (mode == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            cout << i;
            for (int j = 0; j < 8; j++)
            {
                if (base[j][i])
                {
                    cout << Print_base;
                }
                else
                {
                    cout << Print_base;
                }
            }
            cout << endl;
        }
        cout << " ";
        for (int i = 0; i < 8; i++)
        {
            cout << i;
        }
        cout << endl;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            cout << i;
            for (int j = 0; j < 4; j++)
            {
                if (temp[i][j])
                {
                    cout << Print_base;
                }
                else
                {
                    cout << " ";
                }
            }
            cout << endl;
        }
        cout << " ";
        for (int i = 0; i < 4; i++)
        {
            cout << i;
        }
        cout << endl;
    }
}
//输出model形状,clean判定是否清除,true将清除(输出空格)
void model::print_model(bool clean)
{
    color(Color);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (temp[i][j])
            {
                if (clean)
                {
                    cout << " ";
#ifdef _WIN32
                    cursor_location++;
#endif
                }
                else
                {
                    cout << Print_base;
#ifdef _WIN32
                    cursor_location++;
#endif
                }
            }
            else
            {
                moveright(1);
            }
        }
        movedown(1);
        moveleft(length);
    }
    cout.flush();
}
//以旋转中心旋转ang度，只能是90的倍数
void model::changer_neg(int ang)
{
    int after_x;
    int after_y;
    bool changed_map[8][8];
    memset(changed_map, 0, 64 * sizeof(bool));
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (base[i][j])
            {
                after_x = (int)((i - center_y) * cos(ang * PI / 180) + (j - center_y) * sin(ang * PI / 180) + center_x + 0.5);  //cos(90度)，sin(90度)
                after_y = (int)(-(i - center_x) * sin(ang * PI / 180) + (j - center_y) * cos(ang * PI / 180) + center_y + 0.5); //sin(90度)，cos(90度)
                changed_map[after_x][after_y] = true;
            }
        }
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            base[i][j] = changed_map[i][j];
        }
    }
    get_temp();
}
void model::get_temp()
{
    int r_min = 0;
    int c_min = 0;
    //r_temp为横变量
    //c_temp为竖变量
    int r_temp = 0;
    int c_temp = 0;
    bool get_c_min = false;
    bool get_r_min = false;
    //获取模型中横坐标和纵坐标的最小值
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (base[i][j] && !get_c_min)
            {
                c_min = i;
                get_c_min = true;
            }
            if (base[j][i] && !get_r_min)
            {
                r_min = i;
                get_r_min = true;
            }
            if (get_c_min && get_r_min)
            {
                break;
            }
        }
    }
    //此处开始计算temp数组的值
    memset(temp, 0, 16 * sizeof(bool));
    for (int i = r_min; i < 8; i += 2)
    {
        for (int j = c_min; j < 8; j += 2)
        {
            if (base[j][i])
            {
                temp[r_temp][c_temp++] = true;
            }
            else
            {
                c_temp++;
            }
        }
        c_temp = 0;
        r_temp++;
    }
    height = 0;
    length = 0;
    //此处开始计算length以及height
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (temp[i][j])
            {
                if (i > height)
                {
                    height = i;
                }
            }
            if (temp[j][i])
            {
                if (i > length)
                {
                    length = i;
                }
            }
        }
    }
    //循环只是得到数组中的最大值，实际得加一
    length += 1;
    height += 1;
}
int model::get_color()
{
    return Color;
}