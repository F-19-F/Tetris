#include <iostream>
#include <thread>
#include <memory.h>
using namespace std;
#include "include/game_core.h"
#include "include/terminal-linux.h"
//删除临时结果
void game_core::del_base()
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
//
void game_core::debug_core()
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (rand() % 2)
                *(source + i * y + j) = true;
        }
        if (i == 3 || i == 6 || i == 4)
        {
            for (int j = 0; j < y; j++)
            {
                *(source + i * y + j) = true;
            }
        }
    }
}
//输出当前表
void game_core::print()
{
    //打印边框
    //打印竖直方向的边框
    //red_foreground();
    //blue_background();
    //clean_screen();
    for (int i = 0; i < x; i++)
    {
        cursor_move(1, x - i + 1);
        cout << "|";
        cursor_move(y + 2, x - i + 1);
        cout << "|";
    }
    //打印横方向的边框
    for (int i = 0; i < y; i++)
    {
        cursor_move(2 + i, 1);
        cout << "-";
        cursor_move(2 + i, x + 2);
        cout << "-";
    }
    cursor_move(1, 1);
    cout << "┌";
    cursor_move(1, y);
    cout << "└";
    cursor_move(y + 2, 1);
    cout << "┐";
    cursor_move(y + 2, x + 2);
    cout << "┘";
    //end_all();
    ///for linux/unix terminal////
    //yellow_background();
    blue_foreground();
    hide_cursor();
    ///for linux/unix terminal////
    cout.flush();
    for (int i = 0; i < Min_R(); i++) //从0行开始到x-1行
    {
        cursor_move(2, x - i + 1); //+1是为了给边框空行
        for (int j = 0; j < y; j++)
        {
            if (*(source + i * y + j))
            {
                cout << Print_base;
                cout.flush();
            }
            else
            {
                cout << " ";
                //printf(" ");
            }
        }
        //cursor_move(y+3, x - i + 1);
        //cout << "--------line" << i + 1;
        //cout << endl;
    }
    end_all();
    cout.flush();
}
//初始化x表示能能够占有的行，y表示能够占用的列,speed为模型下落速度
game_core::game_core(int x, int y,int speed)
{
    this->x = x - 2; //方便打印边框
    this->y = y;
    source = new bool[x * y];                //new []为分配多少个空间，（）为分配一个空间并初始化内容为()中的数
    memset(source, 0, x * y * sizeof(bool)); //将方块全部填充为0
    this->speed=speed;
}
game_core::~game_core()
{
    delete[] source;
}
//删除表中现有空行---辅助函数
void game_core::flash()
{
    int r_temp;
    int empty_made = 0;
    for (int i = 0; i < x - empty_made; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (*(source + i * y + j) == true)
                break;
            //将空行压缩
            if (j == y - 1)
            {
                r_temp = i;
                for (int m = i + 1; m < x; m++)
                {
                    for (int n = 0; n < y; n++)
                    {
                        *(source + r_temp * y + n) = *(source + m * y + n);
                    }
                    r_temp = m;
                }
                //将原来最后一行变为空行
                for (int n = 0; n < y; n++)
                {
                    *(source + (x - 1) * y + n) = false;
                }
                //行循环更改
                empty_made++;
                i--;
            }
        }
    }
}
//删除clean所返回的需要删除的行--辅助函数
void game_core::R_delete()
{
    clean_base *target;
    target = temp;
    target = target->next;
    int r_temp;
    while (target != NULL)
    {
        //重置为0
        r_temp = target->location;
        for (int i = 0; i < y; i++)
        {
            *(source + r_temp * y + i) = false;
        }
        target = target->next;
    }
    flash();
}
//执行一次，满行全部删除
int game_core::clean()
{
    int sum = 0;
    clean_base *temp_fun;
    temp_fun = new clean_base;
    temp_fun->next = NULL;
    this->temp = temp_fun;
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (*(source + i * y + j) == false)
            {
                break;
            }
            if (j == y - 1)
            {
                temp_fun->next = new clean_base;
                temp_fun->next->location = i;
                temp_fun->next->next = NULL;
                temp_fun = temp_fun->next;
                sum++;
            }
        }
    }
    R_delete();
    //避免内存泄漏
    del_base();
    return sum;
}
//返回行标最小的空行
int game_core::Min_R()
{
    int i, j;
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < y; j++)
        {
            if (*(source + i * y + j) == true)
            {
                break;
            }
        }
        if (j == y)
        {
            return i;
        }
    }
    if (i == x)
    {
        return x;
    }
    return -1;
}
void game_core::Add_model(model *target)
{
    clean_screen();
    print();
    cout.flush();
    //第二行开始是考虑到了上边界
    //cursor_move(y/2,2);
    //target->print_model();
    //由于开头已经打印一次，故将重新更换位置
    for (int i=x;i>=Min_R()+target->get_height();i--)
    {
        clean_screen();
        print();
        cout.flush();
        cursor_move(y/2,x-i+2);
        //target->changer_neg();
        target->print_model();
        cout.flush();
        this_thread::sleep_for(std::chrono::milliseconds(200));//c++特有的休眠方式
    }
}