#include <iostream>
#include "include/Tetris_Core.hpp"
#include <memory.h>
using namespace std;
//初始化r表示能能够占有的行，c表示能够占用的列,speed为模型下落速度
Tetris_Core::Tetris_Core(int r, int c, int x_offset, int y_offset, int speed , bool Auto_increase)
{
    this->r = r - 2; //方便打印边框
    this->c = c - 2;
    this->x_offset = x_ini_offset + x_offset;
    this->y_offset = y_ini_offset + y_offset;
    over = false;
    this->Auto_Increase=Auto_increase;
    score = 0;
    Died_Line=0;
    source = new bool[r * c]; //new []为分配多少个空间，（）为分配一个空间并初始化内容为()中的数
    Color = new char[r * c];
    memset(source, 0, r * c * sizeof(bool)); //将方块全部填充为0
    memset(Color, 0, r * c * sizeof(char));
    this->speed = speed;
}
Tetris_Core::~Tetris_Core()
{
    delete[] source;
    delete[] Color;
}
//通过压缩空行两边的实现隐藏空行---辅助函数
void Tetris_Core::Hide_Empty_Line()
{
    int r_temp;
    int empty_made = 0;
    for (int i = 0; i < r - empty_made; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (*(source + i * c + j) == true)
                break;
            //将空行压缩
            if (j == c - 1)
            {
                r_temp = i;
                for (int m = i + 1; m < r; m++)
                {
                    for (int n = 0; n < c; n++)
                    {
                        *(source + r_temp * c + n) = *(source + m * c + n);
                        *(Color + r_temp * c + n) = *(Color + m * c + n);
                    }
                    r_temp = m;
                }
                //将原来最后一行变为空行
                for (int n = 0; n < c; n++)
                {
                    *(source + (r - 1) * c + n) = false;
                    *(Color + (r - 1) * c + n) = 0;
                }
                //行循环更改
                empty_made++;
                i--;
            }
        }
    }
}
//删除临时结果--辅助函数
void Tetris_Core::Clean_base_cache()
{
    clean_base *target;
    target = this->temp;
    clean_base *temp_fun;
    while (target != NULL)
    {
        temp_fun = target->next;
        delete target;
        target = temp_fun;
    }
    temp = NULL;
}
//返回行标最小的空行--辅助函数
int Tetris_Core::Min_R()
{
    int i, j;
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            if (*(source + i * c + j) == true)
            {
                break;
            }
        }
        if (j == c)
        {
            return i;
        }
    }
    if (i == r)
    {
        return r;
    }
    return -1;
}
//删除clean所返回的需要删除的行--辅助函数
void Tetris_Core::R_delete()
{
    clean_base *target;
    target = temp;
    target = target->next;
    int r_temp;
    while (target != NULL)
    {
        //重置为0
        r_temp = target->location;
        for (int i = 0; i < c; i++)
        {
            *(source + r_temp * c + i) = false;
        }
        target = target->next;
    }
}
//Can_move函数的输入x,y是终端原始坐标
bool Tetris_Core::Can_move_down(int x, int y, model *target)
{
    x -= x_offset;
    y -= y_offset;
    //模块往下移一行到达底部时将直接返回false
    if ((y + (target->height)) == r + 2)
    {
        return false;
    }
    //从模块最底层开始检索
    for (int i = target->Current_Print_Line - 1; i >= 0; i--)
    {
        for (int j = 0; j < target->length; j++)
        {
            //检测到模块方格所在位置
            if (target->temp[i][j])
            {
                if (*(source + (r + 1 - (y + i + 1)) * c + (x + j - 2)))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
bool Tetris_Core::Can_move_left(int x, int y, model *target)
{
    x -= x_offset;
    y -= y_offset;
    //从模型最左边起检索
    if (x == 2)
    {
        return false;
    }
    for (int j = 0; j < target->length; j++)
    {
        for (int i = target->height - 1; i >= 0; i--)
        {
            if (target->temp[i][j])
            {
                if (*(source + (r + 1 - (y + i)) * c + (x + j - 3)))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
bool Tetris_Core::Can_move_right(int x, int y, model *target)
{
    x -= x_offset;
    y -= y_offset;
    //从模型最右边起检索
    if (x + target->length == c + 2)
    {
        return false;
    }
    for (int j = target->length - 1; j >= 0; j--)
    {
        for (int i = target->height - 1; i >= 0; i--)
        {
            if (target->temp[i][j])
            {
                if (*(source + (r + 1 - (y + i)) * c + (x + j - 1)))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
//判断当前模型的位置是否符合规范，用于判断模型是否能旋转
bool Tetris_Core::Is_valid(int x, int y, model *target)
{
    x -= x_offset;
    y -= y_offset;
    //超出右边界不行
    if (x + target->length > c + 2)
    {
        return false;
    }
    //超出下边界不行
    if (y + target->height > r + 1)
    {
        return false;
    }
    for (int j = target->length - 1; j >= 0; j--)
    {
        for (int i = target->height - 1; i >= 0; i--)
        {
            if (target->temp[i][j])
            {
                //检测是否有冲突
                if (*(source + (r + 1 - (y + i)) * c + (x + j - 2)))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
//将当前模型写入核心
void Tetris_Core::Write_core(int x, int y, model *target)
{
    x -= x_offset;
    y -= y_offset;
    for (int i = target->height - 1; i >= 0; i--)
    {
        for (int j = 0; j < target->length; j++)
        {
            if (target->temp[i][j])
            {
                *(source + (r + 1 - (y + i)) * c + (x + j - 2)) = true;
                *(Color + (r + 1 - (y + i)) * c + (x + j - 2)) = target->get_color();
            }
        }
    }
}
//接口函数，外界能从这里得到游戏信息
int Tetris_Core::get_score()
{
    return score;
}
int Tetris_Core::get_speed()
{
    return speed;
}
int Tetris_Core::Get_R()
{
    return r;
}
int Tetris_Core::Get_C()
{
    return c;
}
void Tetris_Core::Edit_Color(char *RColor)
{
    Color=RColor;
}
void Tetris_Core::Edit_Source(bool *Rsource)
{
    source=Rsource;
}
void Tetris_Core::Editoffset(int x_offset,int y_offset)
{
    this->x_offset=x_offset;
    this->y_offset=y_offset;
}