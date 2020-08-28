#include <iostream>
#include <thread>
#include <memory.h>
#include <mutex>
using namespace std;
#include "include/game_core.hpp"
#ifndef _WIN32
#include "include/ANSI_control.hpp"
bool compat_mode = true;
void SetColorCompat(bool opt)
{
    compat_mode = opt;
    return;
}
#else
#include "include/WinAPI_control.hpp"
#endif
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
    for (int i = 0; i < 7; i++)
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
    int C_temp = 0;
    //打印边框
    //打印竖直方向的边框
    color(7);
    for (int i = 0; i < r; i++)
    {
        cursor_move(1 + x_offset, r - i + 1 + y_offset);
        cout << "│";
        cursor_move(c + 2 + x_offset, r - i + 1 + y_offset);
        cout << "│";
    }
    //打印横方向的边框
    for (int i = 0; i < c; i++)
    {
        cursor_move(2 + i + x_offset, 1 + y_offset);
        cout << "─";
        cursor_move(2 + i + x_offset, r + 2 + y_offset);
        cout << "─";
    }
    cursor_move(1 + x_offset, 1 + y_offset);
    cout << "┌";
    cursor_move(1 + x_offset, r + 2 + y_offset);
    cout << "└";
    cursor_move(c + 2 + x_offset, 1 + y_offset);
    cout << "┐";
    cursor_move(c + 2 + x_offset, r + 2 + y_offset);
    cout << "┘";
    //blue_foreground();
    hide_cursor();
    cout.flush();
    for (int i = 0; i < Min_R(); i++) //从0行开始到r-1行
    {
        cursor_move(2 + x_offset, r - i + 1 + y_offset); //+1是为了给边框空行
        for (int j = 0; j < c; j++)
        {
            if (*(source + i * c + j))
            {
                if (C_temp != *(Color + i * c + j))
                {
                    color(*(Color + i * c + j));
                    C_temp = *(Color + i * c + j);
                }
                cout << (Print_base);
#ifdef _WIN32
                cursor_location++;
#endif
            }
            else
            {
                cout << " ";
#ifdef _WIN32
                cursor_location++;
#endif
            }
        }
    }
    cout.flush();
}
//初始化x表示能能够占有的行，c表示能够占用的列,speed为模型下落速度
game_core::game_core(int r, int c, int x_offset, int y_offset, int speed)
{
    this->r = r - 2; //方便打印边框
    this->c = c - 2;
    this->x_offset = x_ini_offset + x_offset;
    this->y_offset = y_ini_offset + y_offset;
    over = false;
    score = 0;
    source = new bool[r * c]; //new []为分配多少个空间，（）为分配一个空间并初始化内容为()中的数
    Color = new int[r * c];
    memset(source, 0, r * c * sizeof(bool)); //将方块全部填充为0
    memset(Color, 0, r * c * sizeof(int));
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
    int Before_min = Min_R();
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
    draw_delline();
    del_base();
    cursor_move(2 + x_offset, r + 2 - Before_min + y_offset);
    //清除多余行
    for (int i = 0; i < sum; i++)
    {
        for (int j = 0; j < c; j++)
        {
            cout << " ";
#ifdef _WIN32
            cursor_location++;
#endif
        }
        moveleft(c);
        movedown(1);
    }
    return sum;
}
//返回行标最小的空行--辅助函数
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
//Move函数主管方块正常下落中对按键的反馈，x,y为模块当前坐标，signal为同步信号，target为模型对象,core为游戏core对象，Key为按键输入对象
//signal为0时线程将正常运行
//signal为1时将结束控制线程以及Move线程
void game_core::draw_delline()
{
    clean_base *target;
    target = temp;
    target = target->next;
    int r_temp;
    if (target == NULL)
    {
        return;
    }
    for (int j = 0; j < 3; j++)
    {
        while (target != NULL)
        {
            //重置为0
            r_temp = target->location;
            cursor_move(2 + x_offset, r + 1 - r_temp + y_offset);
            for (int i = 0; i < c; i++)
            {
                cout << " ";
#ifdef _WIN32
                cursor_location++;
#endif
                cout.flush();
            }
            cout.flush();
            target = target->next;
        }
        this_thread::sleep_for(std::chrono::milliseconds(200));
        target = temp->next;
        while (target != NULL)
        {
            //重置为0
            r_temp = target->location;
            cursor_move(2 + x_offset, r + 1 - r_temp + y_offset);
            for (int i = 0; i < c; i++)
            {
                cout << Print_base;
#ifdef _WIN32
                cursor_location++;
#endif
                cout.flush();
            }
            cout.flush();
            target = target->next;
        }
        target = temp->next;
        this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    target = temp->next;
    while (target != NULL)
    {
        //重置为0
        r_temp = target->location;
        cursor_move(2 + x_offset, r + 1 - r_temp + y_offset);
        for (int i = 0; i < c; i++)
        {
            cout << " ";
            cout.flush();
#ifdef _WIN32
            cursor_location++;
#endif
        }
        cout.flush();
        target = target->next;
    }
}
//signal为线程间同步变量，为-1时则按键响应线程结束，为0时代表需要一个下降的delay，为1时则正常运行
void Move(int *x, int *y, int *signal, model *target, mutex *ctrl, Key_dec *Key, game_core *core, mutex *Lock)
{
    bool suspend = false;
    while (ctrl->try_lock())
    {
        ctrl->unlock();
        Key->MutexLock(true);
        if (suspend)
        {
            if (Key->pop() == space)
            {
                Lock->unlock();
                suspend = false;
            }
        }
        else
        {
            switch (Key->pop())
            {
            case left:
                if (core->Can_move_left(*x, *y, target))
                {
                    Lock->lock();
                    //如果下落线程已经结束，则移动全部取消
                    if (!ctrl->try_lock())
                    {
                        Lock->unlock();
                        Key->MutexLock(false);
                        *signal = 2;
                        return;
                    }
                    else
                    {
                        ctrl->unlock();
                    }
                    cursor_move(*x, *y);
                    target->print_model(true);
                    *x = *x - 1;
                    cursor_move(*x, *y);
                    target->print_model(false);
                    *signal = 0;
                    Lock->unlock();
                }
                break;
            case right:
                if (core->Can_move_right(*x, *y, target))
                {
                    Lock->lock();
                    //如果下落线程已经结束，则移动全部取消
                    if (!ctrl->try_lock())
                    {
                        Lock->unlock();
                        Key->MutexLock(false);
                        *signal = 2;
                        return;
                    }
                    else
                    {
                        ctrl->unlock();
                    }
                    cursor_move(*x, *y);
                    target->print_model(true);
                    *x = *x + 1;
                    cursor_move(*x, *y);
                    target->print_model(false);
                    *signal = 0;
                    Lock->unlock();
                }
                break;
            case up:
                Lock->lock();
                //如果下落线程已经结束，则移动全部取消
                if (!ctrl->try_lock())
                {
                    Lock->unlock();
                    Key->MutexLock(false);
                    *signal = 2;
                    return;
                }
                else
                {
                    ctrl->unlock();
                }
                cursor_move(*x, *y);
                target->print_model(true);
                target->changer_neg(-90);
                //如果不符合要求,就撤回更改，同时不发送relay请求
                if (!core->Is_valid(*x, *y, target))
                {
                    target->changer_neg(90);
                    *signal = 1;
                }
                else
                {
                    *signal = 0;
                }
                cursor_move(*x, *y);
                target->print_model(false);
                Lock->unlock();
                break;
            case down:
                Lock->lock();
                //如果下落线程已经结束，则移动全部取消
                if (!ctrl->try_lock())
                {
                    Lock->unlock();
                    Key->MutexLock(false);
                    *signal = 2;
                    return;
                }
                else
                {
                    ctrl->unlock();
                }
                cursor_move(*x, *y);
                target->print_model(true);
                if (core->Can_move_down(*x, *y, target))
                {
                    *y = *y + 1;
                    if (core->Can_move_down(*x, *y, target))
                    {
                        *y = *y + 1;
                    }
                }
                cursor_move(*x, *y);
                target->print_model(false);
                Lock->unlock();
                break;
            case space:
                suspend = true;
                Lock->lock();
                break;
            default:
                break;
            }
        }
        Key->MutexLock(false);
        this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    *signal = 2; //告诉主线程，此线程已结束，无需再等待
    return;
}
void game_core::Add_model(model *target, Key_dec *Key)
{
    mutex y_lock;
    mutex Run_Lock;
    int y = 2 + y_offset;
    int x = c / 2 + x_offset;
    int signal = 1;
    int Time_speed = MAX_TIME / speed;
    y_lock.lock();
    thread t1(Move, &x, &y, &signal, target, &Run_Lock, Key, this, &y_lock);
    t1.detach();
    //打印最开始的模型
    cursor_move(x, y);
    target->print_model(false);
    this_thread::sleep_for(std::chrono::milliseconds(Time_speed));
    cursor_move(x, y);
    target->print_model(true);
    y_lock.unlock();
    //最开始模型打印结束,开始正常下落
    while (1)
    {
    Dead_Loop:
        if (Can_move_down(x, y, target))
        {
            //当如果Move在响应时，下落将等待其结束，以防止光标位置错乱导致的奇怪输出
            y_lock.lock();
            y++;
            cursor_move(x, y);
            target->print_model(false);
            y_lock.unlock();
            this_thread::sleep_for(std::chrono::milliseconds(Time_speed));
            //避免下落时发生变形导致的打印异常
            y_lock.lock();
            cursor_move(x, y);
            target->print_model(true);
            y_lock.unlock();
        }
        else
        {
            y_lock.lock();
            if (signal == 0) //写入delay,在检测到delay请求时则延缓写入，并且在delay阶段允许左右移动
            {
                signal = 1;
                y_lock.unlock();
                this_thread::sleep_for(std::chrono::milliseconds(30));
                y_lock.lock();
                //如果delay阶段的移动使得方块能继续下降，则继续下降
                if (Can_move_down(x, y, target))
                {
                    //消除delay阶段所打印的方块
                    cursor_move(x, y);
                    target->print_model(true);
                    y_lock.unlock();
                    goto Dead_Loop;
                }
            }
            Write_core(x, y, target);
            signal = -1;
            print();
            y_lock.unlock();
            break;
        }
    }
    Run_Lock.lock();
    t1.~thread();
    Key->clean();
    score += clean() * speed * (c - 2);
    //检测游戏是否结束
    for (int l = 0; l < c; l++)
        if (*(source + (r - 1) * c + l))
        {
            over = true;
        }
    while (signal != 2)
    {
        this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    Run_Lock.unlock();
    this_thread::sleep_for(std::chrono::milliseconds(100));
}
//Can_move函数的输入x,y是终端原始坐标
bool game_core::Can_move_down(int x, int y, model *target)
{
    x -= x_offset;
    y -= y_offset;
    //模块往下移一行到达底部时将直接返回false
    if ((y + (target->height)) == r + 2)
    {
        return false;
    }
    //从模块最底层开始检索
    for (int i = target->height - 1; i >= 0; i--)
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
bool game_core::Can_move_left(int x, int y, model *target)
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
                    //debug
                    //*(source + (r + 2 - (*y + i)) * c + (*x + j - 3))=false;
                    return false;
                }
            }
        }
    }
    return true;
}
bool game_core::Can_move_right(int x, int y, model *target)
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
                    //debug
                    //*(source + (r + 2 - (*y + i)) * c + (*x + j - 3))=false;
                    return false;
                }
            }
        }
    }
    return true;
}
void game_core::Write_core(int x, int y, model *target)
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
bool game_core::Is_valid(int x, int y, model *target)
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
int game_core::get_score()
{
    return score;
}
int game_core::get_speed()
{
    return speed;
}