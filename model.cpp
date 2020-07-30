#include <iostream>
#include <cmath>
#include "include/terminal-linux.h"
#include "include/model.h"
#include "memory.h"
using namespace std;
//方块模型,mode表示方块类型，1-5将构建预设类型，0代表自主构建
model::model(int mode)
{
    memset(base, 0, 64 * sizeof(bool));
    switch (mode)
    {
    case 1:
        base[3][0] = true; //  ####
        base[3][2] = true; //  ########
        base[3][4] = true; //  ########
        base[5][2] = true; //  ####
        center_x = 3;
        center_y = 2;
        break;
    case 2:
        base[1][2] = true; //  ######
        base[3][2] = true; //  ######
        base[3][4] = true; //      ######
        base[5][4] = true; //      ######
        center_x = 3;
        center_y = 3;
        break;
    case 3:
        base[0][0] = true; //  ########
        base[2][0] = true; //  ########
        base[0][2] = true; //  ########
        base[2][2] = true; //  ########
        center_x = 1;
        center_y = 1;
        break;
    case 4:
        base[3][0] = true; //  ##
        base[3][2] = true; //  ##
        base[3][4] = true; //  ##
        base[3][6] = true; //  ##
        center_x = 3;
        center_y = 3;
        break;
    case 5:
        base[1][3] = true; //          ####
        base[3][3] = true; //          ####
        base[5][3] = true; //  ############
        base[5][1] = true; //  ############
        center_x = 3;
        center_y = 3;
        break;
    default:
        break;
    }
}
//输出原始数据，以便调试--for debug
void model::print_row()
{
    for (int i = 0; i < 8; i++)
    {
        cout << i;
        for (int j = 0; j < 8; j++)
        {
            if (base[j][i])
            {
                cout << "#";
            }
            else
            {
                cout << " ";
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
//输出model形状,clean判定是否清除,true将清除(输出空格)
void model::print_model(bool clean)
{
    //clean_screen();
    //cout.flush();
    //代表某一行已经打印的个数，方便用移动光标的方法换行
    int R_printed;
    bool end_signal = false;
    bool base_map_black[8][8];
    int temp;
    int r_min = 0;
    int c_min = 0;
    bool get_c_min = false;
    bool get_r_min = false;
    //获取模型中横坐标和纵坐标的最小值
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (base[i][j] && !get_c_min)
            {
                r_min = i;
                get_c_min = true;
            }
            if (base[j][i] && !get_r_min)
            {
                c_min = i;
                get_r_min = true;
            }
            if (get_c_min && get_r_min)
            {
                break;
            }
        }
    }
    memset(base_map_black, 0, 64 * sizeof(bool));
    for (int j = c_min; j < 8; j++)
    {
        R_printed = 0;
        for (int i = r_min; i < 8; i++)
        {
            //伪记忆搜索
            if (base_map_black[i][j])
                continue;
            if (base[i][j] == true)
            {
                temp = i - 2;
                while (temp >= r_min)
                {
                    //有效格为空时则打印空格，以显示其相对位置关系
                    //7.30修改，改为移动光标
                    if (base[temp][j])
                        break;
                    if (!base[temp][j])
                    {
                        //cout << " ";
                        moveright(1);
                        R_printed++;
                    }
                    temp -= 2;
                }
                end_signal = true;
                if (clean)
                {
                    cout << " ";
                }
                else
                {
                    cout << "#";
                }
                //cout << "#";
                R_printed++;
                cout.flush();
                if (i - 1 >= 0)
                {
                    base_map_black[i - 1][j] = true;
                }
                if (j - 1 >= 0)
                {
                    base_map_black[i][j - 1] = true;
                }
                if (i + 1 < 8)
                {
                    base_map_black[i + 1][j] = true;
                }
                if (j + 1 < 8)
                {
                    base_map_black[i][j + 1] = true;
                }
                if (i - 1 >= 0 && j - 1 >= 0)
                {
                    base_map_black[i - 1][j - 1] = true;
                }
                if (i + 1 < 8 && j + 1 < 8)
                {
                    base_map_black[i + 1][j + 1] = true;
                }
                if (i - 1 >= 0 && j + 1 < 8)
                {
                    base_map_black[i - 1][j + 1] = true;
                }
                if (i + 1 < 8 && j - 1 >= 0)
                {
                    base_map_black[i + 1][j - 1] = true;
                }
            }
        }
        if (end_signal)
        {
            //cout << endl;
            movedown(1);
            moveleft(R_printed);
            end_signal = false;
        }
    }
    cout.flush();
}
//以旋转中心逆时针旋转90度
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
}
//返回模型的占用的高
int model::get_height()
{
    int sum = 0;
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            if (base[i][j])
            {
                sum++;
                break;
            }
        }
    }
    return sum;
}
//返回模型占用的宽
int model::get_length()
{
    int sum = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (base[i][j])
            {
                sum++;
                break;
            }
        }
    }
    return sum;
}