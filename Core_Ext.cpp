#include <iostream>
#include <fstream>
#include <thread>
#include <memory.h>
#include <mutex>
using namespace std;
#include "include/Tetris_Core.hpp"
#include "include/UI.hpp"
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
#define Print_Current           \
    cursor_move(x, y);          \
    target->print_model(false); \
    cout.flush();
#define Clean_Current  \
    cursor_move(x, y); \
    target->print_model(true);
#define Print_Current_P         \
    cursor_move(*x, *y);        \
    target->print_model(false); \
    cout.flush();
#define Clean_Current_P  \
    cursor_move(*x, *y); \
    target->print_model(true);
#define Can_I_Run          \
    if (!ctrl->try_lock()) \
    {                      \
        Lock->unlock();    \
        *signal = 2;       \
        return;            \
    }                      \
    else                   \
    {                      \
        ctrl->unlock();    \
    }
#define workout_address                   \
    address += (address_temp[0]) * 65536; \
    address += (address_temp[1]) * 256;   \
    address += (address_temp[2]);
//输出当前表
void Tetris_Core::Core_Print()
{
    char C_temp = 0;
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
    for (int i = 0; i < Min_R(); i++) //从0行开始到最小空行前
    {
        cursor_move(2 + x_offset, r - i + 1 + y_offset); //+1是为了给边框空行
        for (int j = 0; j < c; j++)
        {
            if (*(source + i * c + j))
            {
                if (C_temp != *(Color + i * c + j))
                {
                    color((int)*(Color + i * c + j));
                    C_temp = *(Color + i * c + j);
                }
                cout << (Print_base);
            }
            else
            {
                cout << " ";
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
    for (int i = 0; i < Min_R(); i++)
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
        }
        cout.flush();
        target = target->next;
    }
}
//signal为线程间同步变量，为0时代表需要一个下降的delay，为1时则正常运行,为2时代表线程已结束(或者用户主动退出)
void Move(int *x, int *y, int *signal, model *target, mutex *ctrl, Key_dec *Key, Tetris_Core *core, mutex *Lock, Tetris_UI *UI)
{
    bool suspend = false;
    int Key_Got;
    while (ctrl->try_lock())
    {
        ctrl->unlock();
        Key->MutexLock(true);
        Key_Got = Key->pop();
        Key->MutexLock(false);
        if (suspend)
        {
            if (Key_Got == space)
            {
                UI->Infor(false);
                Lock->unlock();
                suspend = false;
            }
            if (Key_Got == esc)
            {
                if (UI->Dialog((char *)"是", (char *)"否", (char *)"警告", (char *)"是否退出？\n") == 1)
                {
                    *signal = 2;
                    Lock->unlock();
                    break;
                }
                else
                {
                    clean_screen();
                    core->Core_Print();
                    UI->Infor();
                    Print_Current_P;
                }
            }
        }
        else
        {
            switch (Key_Got)
            {
            case left:
                Lock->lock();
                if (core->Can_move_left(*x, *y, target))
                {
                    //如果下落线程已经结束，则移动全部取消
                    Can_I_Run;
                    Clean_Current_P;
                    *x = *x - 1;
                    Print_Current_P;
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
                    Can_I_Run;
                    Clean_Current_P;
                    *x = *x + 1;
                    Print_Current_P;
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
                Can_I_Run;
                Clean_Current_P;
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
                Print_Current_P;
                Lock->unlock();
                break;
            case down:
                Lock->lock();
                //如果下落线程已经结束，则移动全部取消
                Can_I_Run;
                Clean_Current_P;
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
                Print_Current_P;
                *signal = 1;
                Lock->unlock();
                break;
            case space:
                Lock->lock();
                core->Save_To_file((char *)OutPutName);
                Can_I_Run;
                suspend = true;
                UI->Infor(true);
                break;
            default:
                break;
            }
        }
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
    int Score_In = 0;
    int Time_speed = MAX_TIME / speed;
    y_lock.lock();
    thread t1(Move, &x, &y, &signal, target, &Run_Lock, Key, this, &y_lock, _UI);
    t1.detach();
    y_lock.unlock();
    while (1)
    {
    Dead_Loop:
        if (Can_move_down(x, y, target))
        {
            //当如果Move在响应时，下落将等待其结束，以防止光标位置错乱导致的奇怪输出
            y_lock.lock();
            if (signal == 2)
            {
                over = true;
                y_lock.unlock();
                t1.~thread();
                remove(OutPutName);
                return;
            }
            if (target->All_Model())
            {
                y++;
            }
            else
            {
                target->Part_to_temp(target->Current_Print_Line + 1);
            }
            Print_Current;
            y_lock.unlock();
            this_thread::sleep_for(std::chrono::milliseconds(Time_speed));
            //避免下落时发生变形导致的打印异常
            y_lock.lock();
            Clean_Current;
            y_lock.unlock();
        }
        else
        {
            y_lock.lock();
            if (signal == 2)
            {
                over = true;
                y_lock.unlock();
                t1.~thread();
                remove(OutPutName);
                return;
            }
            if (signal == 0) //写入delay,在检测到delay请求时则延缓写入，并且在delay阶段允许左右移动
            {
                signal = 1;
                Print_Current;
                y_lock.unlock();
                this_thread::sleep_for(std::chrono::milliseconds(30));
                y_lock.lock();
                //如果delay阶段的移动使得方块能继续下降，则继续下降
                if (Can_move_down(x, y, target))
                {
                    //消除delay阶段所打印的方块
                    Clean_Current;
                    y_lock.unlock();
                    goto Dead_Loop;
                }
            }
            Run_Lock.lock();
            Print_Current;
            Key->clean();
            y_lock.unlock();
            Write_core(x, y, target);
            while (signal != 2)
            {
                this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            Run_Lock.unlock();
            break;
        }
    }
    t1.~thread();
    if ((Score_In = Full_Line_Clean()) != 0)
    {
        Core_Print();
        score += Score_In * speed * (c - 2);
        if (Auto_Increase)
        {
            while (Score_In--)
            {
                Died_Line++;
                if (!((Died_Line) % 10) && speed < MAX_LEVEL)
                    speed++;
            }
        }
    }
    //检测游戏是否结束
    if (y == 2 + y_offset)
    {
        over = true;
        //有戏结束则关闭文件存储
        Close_Tail((char *)OutPutName);
        remove(OutPutName);
    }
    else
    {
        Save_To_file((char *)OutPutName);
    }
    Run_Lock.unlock();
    if (over)
    {
        this_thread::sleep_for(std::chrono::milliseconds(Time_speed));
    }
}
bool Is_Cofig_file(char *path)
{
    char Temp[2];
    fstream t(path, ios::binary | ios::in);
    if (!t)
    {
        return false;
    }
    t.seekg(-2, ios::end);
    t.read(Temp, 2);
    t.close();
    if ((Temp[0] == 0x0F) && (Temp[1] == 0x19))
    {
        return true;
    }
    return false;
}
//以字节为单位直接写入数据，file为ostream对象
void Direct_Hex(std::ostream &file, long address, int Length)
{
    char *bytes = new char[Length];
    for (int i = 0; i < Length; i++)
    {
        bytes[Length - 1 - i] = (address >> (8 * i)) & 0xFF;
    }
    //将数据写入
    file.write((char *)bytes, Length);
}
bool Close_Tail(char *path)
{
    if (Is_Cofig_file(path))
    {
        fstream target(path, ios::out | ios::binary);
        if (!target)
        {
            return false;
        }
        target.seekp(-2, ios::end);
        Direct_Hex(target, 0x0F09, 2);
        target.close();
        return true;
    }
    return false;
}

bool Tetris_Core::Save_To_file(char *path)
{
    bool *temp = source;
    char *Color_temp = Color;
    char *address_temp;
    long address = 0;
    fstream Bak(path, ios::binary | ios::in | ios::app);
    if (!Bak)
    {
        return false;
    }
#ifdef _WIN32
    Hide_File(path);
#endif
    //移动读取指针到当前文件的末尾，以获取文件的大小信息
    Bak.seekg(0, ios::end);
    //存储文件大小参数
    int File_Length = Bak.tellg();
    //将文件读指针移动至文件倒数两个字节
    Bak.close();
    Bak.open(path, ios::out | ios::binary | ios::in);
    if (File_Length < 5)
    {
        goto WRITE;
    }
    Bak.seekg(-2, ios::end);
    address_temp = new char[2];
    Bak.read(address_temp, 2);
    //如果在尾部寻找到特殊字节，则认为是标准的游戏数据文件，将替换原有数据
    if ((address_temp[0] == 0x0F) && (address_temp[1] == 0x19 || address_temp[1] == 0x09))
    {
        Bak.seekg(-5, ios::end);
        delete address_temp;
        address_temp = new char[3];
        //读取3字节地址
        Bak.read(address_temp, 3);
        workout_address;
        Bak.seekp(address, ios::beg);
        File_Length = address;
    }
    else
    {
        //否则移动至文件末尾
        Bak.seekp(0, ios::end);
    }
    delete address_temp;
WRITE:
    //将Tetris_Core对象写入文件，但现在存储的文件中指针的部分只存储了指针的地址
    Bak.write((char *)this, sizeof(*this));
    //将source中的数据以字节为单位存入文件，可以节约空间,以后会推出
    //保存动态分配的内容
    for (int i = 0; i < r * c; i++)
    {
        Bak.write((char *)temp, sizeof(*temp));
        temp++;
    }
    for (int i = 0; i < r * c; i++)
    {
        Bak.write((char *)Color_temp, sizeof(*Color_temp));
        Color_temp++;
    }

    //在当前文件末尾写入Core部分的地址以及对应大小,3个字节保存地址，2个字节保存大小
    Direct_Hex(Bak, File_Length, 3);
    //写入特殊文件尾
    Direct_Hex(Bak, Special_Tail, 2);
    //cout << "Write" << hex << "0x" << hex << File_Length << "at the end of file";
    Bak.close();
    return true;
}
Tetris_Core *Restore_Core(char *path)
{
    char address_temp[3];
    long address = 0;
    bool *restored_map;
    int r;
    int c;
    char *c_restored_map;
    Tetris_Core *Restored = (Tetris_Core *)malloc(sizeof(Tetris_Core));
    fstream restore(path, ios::binary | ios::in);
    //读取特定位置
    restore.seekg(-5, ios::end);
    //读取core地址
    restore.read(address_temp, 3);
    //转换读取的内容
    workout_address;
    //读取Core的地址
    restore.seekg(address, ios::beg);
    restore.read((char *)Restored, sizeof(Tetris_Core));
    //从读取的Core中获取对应读取大小
    r = Restored->Get_R();
    c = Restored->Get_C();
    restored_map = (bool *)malloc(sizeof(bool) * r * c);
    restore.read((char *)restored_map, sizeof(bool) * r * c);
    c_restored_map = (char *)malloc(sizeof(char) * r * c);
    restore.read((char *)c_restored_map, sizeof(char) * r * c);
    //修改指针地址
    Restored->Edit_Source(restored_map);
    Restored->Edit_Color(c_restored_map);
    //读取完成
    return Restored;
}