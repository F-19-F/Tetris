#include <iostream>
#include <thread>
#include <memory.h>
#include <mutex>
using namespace std;
#include "include/game_core.h"
#include "include/terminal-linux.h"
#include "include/keydec.h"
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
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (rand() % 2)
                *(source + i * c + j) = true;
        }
        if (i == 3 || i == 6 || i == 4)
        {
            for (int j = 0; j < c; j++)
            {
                *(source + i * c + j) = true;
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
    for (int i = 0; i < r; i++)
    {
        cursor_move(1, r - i + 1);
        cout << "|";
        cursor_move(c + 2, r - i + 1);
        cout << "|";
    }
    //打印横方向的边框
    for (int i = 0; i < c; i++)
    {
        cursor_move(2 + i, 1);
        cout << "-";
        cursor_move(2 + i, r + 2);
        cout << "-";
    }
    cursor_move(1, 1);
    cout << "┌";
    cursor_move(1, r + 2);
    cout << "└";
    cursor_move(c + 2, 1);
    cout << "┐";
    cursor_move(c + 2, r + 2);
    cout << "┘";
    //end_all();
    ///for linux/unix terminal////
    //yellow_background();
    blue_foreground();
    hide_cursor();
    ///for linux/unix terminal////
    cout.flush();
    for (int i = 0; i < Min_R(); i++) //从0行开始到r-1行
    {
        cursor_move(2, r - i + 1); //+1是为了给边框空行
        for (int j = 0; j < c; j++)
        {
            if (*(source + i * c + j))
            {
                cout << Print_base;
                //cout.flush();
            }
            else
            {
                cout << " ";
                //printf(" ");
            }
        }
        //cursor_move(c+3, x - i + 1);
        //cout << "--------line" << i + 1;
        //cout << endl;
    }
    end_all();
    cout.flush();
}
//初始化x表示能能够占有的行，c表示能够占用的列,speed为模型下落速度
game_core::game_core(int r, int c, int speed)
{
    this->r = r - 2; //方便打印边框
    this->c = c - 2;
    source = new bool[r * c];                //new []为分配多少个空间，（）为分配一个空间并初始化内容为()中的数
    memset(source, 0, r * c * sizeof(bool)); //将方块全部填充为0
    this->speed = speed;
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
                    }
                    r_temp = m;
                }
                //将原来最后一行变为空行
                for (int n = 0; n < c; n++)
                {
                    *(source + (r - 1) * c + n) = false;
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
        for (int i = 0; i < c; i++)
        {
            *(source + r_temp * c + i) = false;
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
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (*(source + i * c + j) == false)
            {
                break;
            }
            if (j == c - 1)
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
//targer为添加的模型指针，signal时监控按下的按键，Press_times对应按下值的指针，control为扫描键盘的控制，用来重置计数器
void game_core::Add_model(model *target, int *signal, int *Press_times)
{
    int x_location = c / 2;
    clean_screen();
    print();
    cout.flush();
    //第二行开始是考虑到了上边界
    for (int i = r; i >= Min_R() + target->get_height(); i--)
    {
        clean_screen();
        print();
        cout.flush();
        cursor_move(x_location, r - i + 2);
        if (*signal)
        {
        again:
            switch (*signal)
            {
            case up:
                target->changer_neg(90);
                if (x_location > c - target->get_length() + 2)
                {
                    target->changer_neg(-90);
                }
                *signal = 1;
                break;
            case left:
                *signal = 1;
                if (x_location > 2)
                {

                    x_location -= *Press_times;
                    //this_thread::sleep_for(std::chrono::milliseconds(80));
                    //*control=1;
                    if (x_location < 2)
                    {
                        x_location = 2;
                    }
                    
                    //cursor_move(x_location, r - i + 2);
                    //target->print_model();
                }
                break;
            case right:
                *signal = 1;
                if (x_location + target->get_length() < c + 2)
                {

                    x_location += *Press_times;
                    //this_thread::sleep_for(std::chrono::milliseconds(80));
                    //*control=1;
                    if (x_location >= c - target->get_length() + 2)
                    {
                        x_location = c - target->get_length() + 2;
                    }
                    //cursor_move(x_location, r - i + 2);
                    //target->print_model();
                }
            default:
                break;
            }
        }
        target->print_model(false);
        cout.flush();
        this_thread::sleep_for(std::chrono::milliseconds(200)); //c++特有的休眠方式
        /*for (int j=0;j<5;j++)
        {
            clean_line();
            moveup(1);
        }*/
        //movedown(5);
    }
    print();
    this_thread::sleep_for(std::chrono::milliseconds(200));
}
/*void game_core::beauty(int* x,int* y,int* key_sig)
{
    
}*/