#include <iostream>
#include <thread>
#include <memory.h>
#include <mutex>
using namespace std;
#include "include/Tetris_Core.hpp"
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
//输出当前表
void Tetris_Core::Core_Print()
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
//执行一次，满行全部删除
int Tetris_Core::Full_Line_Clean()
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
    Hide_Empty_Line();
    Del_SE();
    Clean_base_cache(); //避免内存泄漏
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
//消除时的动画
void Tetris_Core::Del_SE()
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
void Move(int *x, int *y, int *signal, model *target, mutex *ctrl, Key_dec *Key, Tetris_Core *core, mutex *Lock)
{
    bool suspend = false;
    int Model_Line = 0;
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
                Lock->lock();
                if (core->Can_move_left(*x, *y, target))
                {
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
                else
                {
                    Lock->unlock();
                }
                break;
            case right:
                Lock->lock();
                if (core->Can_move_right(*x, *y, target))
                {
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
                else
                {
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
                    if (!target->All_Model())
                    {
                        target->Part_to_temp(target->Current_Print_Line + 1);
                    }
                    else
                    {
                        *y = *y + 1;
                    }
                    if (core->Can_move_down(*x, *y, target))
                    {

                        if (!target->All_Model())
                        {
                            target->Part_to_temp(target->Current_Print_Line + 1);
                        }
                        else
                        {
                            *y = *y + 1;
                        }
                    }
                }
                cursor_move(*x, *y);
                target->print_model(false);
                Lock->unlock();
                break;
            case space:
                Lock->lock();
                if (!ctrl->try_lock())
                {
                    Lock->unlock();
                    Key->MutexLock(false);
                    *signal = 2;
                    return;
                }
                {
                    ctrl->unlock();
                }
                suspend = true;
                break;
            default:
                break;
            }
        }
        Key->MutexLock(false);
        this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    *signal = 2; //告诉主线程，此线程已结束，无需再等待
    return;
}
void Tetris_Core::Add_model(model *target, Key_dec *Key)
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
    y_lock.unlock();
    while (1)
    {
    Dead_Loop:
        if (Can_move_down(x, y, target))
        {
            //当如果Move在响应时，下落将等待其结束，以防止光标位置错乱导致的奇怪输出
            y_lock.lock();
            if (target->All_Model())
            {
                y++;
            }
            else
            {
                target->Part_to_temp(target->Current_Print_Line + 1);
            }
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
            Core_Print();
            break;
        }
    }
    Run_Lock.lock();
    y_lock.unlock();
    t1.~thread();
    Key->clean();
    score += Full_Line_Clean() * speed * (c - 2);
    //检测游戏是否结束
    for (int l = 0; l < c; l++)
        if (*(source + (r - 1) * c + l))
        {
            over = true;
        }
    while (signal != 2)
    {
        this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    Run_Lock.unlock();
}