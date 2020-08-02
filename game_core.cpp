#include <iostream>
#include <thread>
#include <memory.h>
#include <mutex>
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
                cout << (Print_base);
                //cout.flush();
            }
            else
            {
                cout << " ";
                //cout.flush();
            }
        }
        //cursor_move(c+3, x - i + 1);
        //cout<<( "--------line" << i + 1;
        //cout<<( endl;
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
void Move(int *x, int *y, int *signal, model *target, bool ctrl, Key_dec *Key, int c_max)
{
    static bool run = true;
    static int time = 0;
    mutex c_lock;
    if (ctrl)
    {
        while (1)
        {
            if (*signal == 1)
            {
                c_lock.lock();
                run = false;
                c_lock.unlock();
                return;
            }
            this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    else
    {
        //Key_dec *Key=new Key_dec();
        time++;
        while (run)
        {
            //cursor_move (58,time);
            //cout<<"Thread"<<time<<"is runing");
            switch (Key->pop())
            {
            case left:
                if (*x > 2)
                {
                    blank();
                    cursor_move(*x, *y);
                    target->print_model(true);
                    *x = *x - 1;
                    this_thread::sleep_for(std::chrono::milliseconds(20));
                    cursor_move(*x, *y);
                    end_all();
                    target->print_model(false);
                }
                break;
            case right:
                if (*x + target->length <= c_max + 1)
                {
                    blank();
                    cursor_move(*x, *y);
                    target->print_model(true);
                    *x = *x + 1;
                    this_thread::sleep_for(std::chrono::milliseconds(20));
                    cursor_move(*x, *y);
                    end_all();
                    target->print_model(false); 
                }
            break;
            case up:
                cursor_move(*x, *y);
                target->print_model(true);
                target->changer_neg(-90);
                //this_thread::sleep_for(std::chrono::milliseconds(20));
                if (*x + target->length > c_max + 2)
                {
                    target->changer_neg(90);
                }
                cursor_move(*x, *y);
                target->print_model(false);
                break;
            default:
                break;
            }
            this_thread::sleep_for(std::chrono::milliseconds(90));
        }
        run = true;
    }
}
void game_core::Add_model(model *target, Key_dec *Key)
{
    mutex y_lock;
    int y = 2;
    int x = c / 2;
    int signal = 0;
    thread t1(Move, &x, &y, &signal, target, true, Key, c);
    thread t2(Move, &x, &y, &signal, target, false, Key, c);
    t1.detach();
    t2.detach();
    clean_screen();
    cout.flush();
    print();
    this_thread::sleep_for(std::chrono::milliseconds(30));
    while (Can_move(&x,&y,target))
    {
        cursor_move(x, y);
        target->print_model(false);
        this_thread::sleep_for(std::chrono::milliseconds(500));
        y_lock.lock();
        cursor_move(x, y);
        x++;
        x--;
        y++;
        target->print_model(true);
        y_lock.unlock();
    }
    signal = 1;
    t1.~thread();
    t2.~thread();
    Key->clean();
    this_thread::sleep_for(std::chrono::milliseconds(500));
}
//Can_move函数的输入x,y是终端原始坐标
bool game_core::Can_move(int *x,int *y,model *target)
{
    if (*y+target->height==r+3)
    {
        return false;
    }
    //从模块最底层开始检索
    for (int i=3;i>=0;i--)
    {
        for (int j=0;j<4;j++)
        {
            if (target->temp[i][j])
            {
                //y+i;//model对应竖坐标
                //x+j;//model横坐标
                //检测其下方是否有方块
                //故只需要检索纵坐标+1后对应的方块是否为空即可
                //需要检索的竖坐标为y+i+1
                //而source中坐标与内存数组之间的关系为
                //坐标(x,y)对应[x+r-3][y-2]
                //故最终需要检索的内存为[x+j+r-3][y+i-1]
                if (*(source + (*y+i-1) * c + (*x+j+r-3)))
                {
                    return false;
                }
            }
        }
    }
    return true;
}