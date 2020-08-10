#include <iostream>
#include <thread>
#include <memory.h>
#include <mutex>
using namespace std;
#include "include/game_core.hpp"
#include "include/terminal-linux.hpp"
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
    for (int i = 0; i < 2; i++)
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
    over = false;
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
//signal为2时表示正在打印，正常下落线程此时应该等待
void Move(int *x, int *y, int *signal, model *target, bool ctrl, Key_dec *Key, game_core *core,mutex *Lock)
{
    static bool run = true;
    static int time = 0;
    if (ctrl)
    {
        while (1)
        {
            if (*signal == 1)
            {
                run = false;
                return;
            }
            this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    else
    {
        time++;
        while (run)
        {
            switch (Key->pop())
            {
            case left:
                if (core->Can_move_left(x, y, target))
                {
                    Lock->lock();
                    blank();
                    //*signal=2;
                    cursor_move(*x, *y);
                    target->print_model(true);
                    *x = *x - 1;
                    this_thread::sleep_for(std::chrono::milliseconds(20));
                    cursor_move(*x, *y);
                    end_all();
                    target->print_model(false);
                    //*signal=0;
                    Lock->unlock();
                }
                break;
            case right:
                if (core->Can_move_right(x, y, target))
                {
                    Lock->lock();
                    blank();
                    //*signal=2;
                    cursor_move(*x, *y);
                    target->print_model(true);
                    *x = *x + 1;
                    this_thread::sleep_for(std::chrono::milliseconds(20));
                    cursor_move(*x, *y);
                    end_all();
                    target->print_model(false);
                    //*signal=0;
                    Lock->unlock();
                }
                break;
            case up:
                //*signal=2;
                Lock->lock();
                cursor_move(*x, *y);
                target->print_model(true);
                target->changer_neg(-90);
                //this_thread::sleep_for(std::chrono::milliseconds(20));
                //如果不符合要求,就撤回更改
                if (!core->Is_valid(x, y, target))
                {
                    target->changer_neg(90);
                }
                cursor_move(*x, *y);
                target->print_model(false);
                //*signal=0;
                Lock->unlock();
                break;
            case down:
                //*signal=2;
                Lock->lock();
                cursor_move(*x, *y);
                target->print_model(true);
                if (core->Can_move_down(x, y, target))
                {
                    *y = *y + 1;
                    if (core->Can_move_down(x, y, target))
                    {
                        *y = *y + 1;
                    }
                }
                cursor_move(*x, *y);
                target->print_model(false);
                Lock->unlock();
                //*signal=0;
                break;
            default:
                break;
            }
            this_thread::sleep_for(std::chrono::milliseconds(30));
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
    thread t1(Move, &x, &y, &signal, target, true, Key, this,&y_lock);
    thread t2(Move, &x, &y, &signal, target, false, Key, this,&y_lock);
    t1.detach();
    t2.detach();
    //打印最开始的模型
    y_lock.lock();
    cursor_move(x, y);
    target->print_model(false);
    this_thread::sleep_for(std::chrono::milliseconds(500));
    cursor_move(x, y);
    target->print_model(true);
    y_lock.unlock();
    //最开始模型打印结束,开始正常下落
    while (1)
    {
        if (Can_move_down(&x, &y, target))
        {
            //当如果Move在响应时，下落将等待其结束，以防止光标位置错乱导致的奇怪输出
            y_lock.lock();
            y++;
            cursor_move(x, y);
            target->print_model(false);
            y_lock.unlock();
            this_thread::sleep_for(std::chrono::milliseconds(500));
            //避免下落时发生变形导致的打印异常
            y_lock.lock();
            cursor_move(x, y);
            target->print_model(true);
            y_lock.unlock();
        }
        else
        {
            Write_core(x, y, target);
            break;
        }
    }
    signal = 1;
    t1.~thread();
    t2.~thread();
    Key->clean();
    clean();
    //检测游戏是否结束
    for (int l = 0; l < c; l++)
        if (*(source + (r - 1) * c + l))
        {
            over = true;
        }
    this_thread::sleep_for(std::chrono::milliseconds(100));
}
//Can_move函数的输入x,y是终端原始坐标
bool game_core::Can_move_down(int *x, int *y, model *target)
{
    //模块往下移一行到达底部时将直接返回false
    if ((*y + (target->height)) == r + 2)
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
                if (*(source + (r + 1 - (*y + i + 1)) * c + (*x + j - 2)))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
bool game_core::Can_move_left(int *locat_x, int *locat_y, model *target)
{
    //从模型最左边起检索
    if (*locat_x == 2)
    {
        return false;
    }
    for (int j = 0; j < target->length; j++)
    {
        for (int i = target->height - 1; i >= 0; i--)
        {
            if (target->temp[i][j])
            {
                if (*(source + (r + 1 - (*locat_y + i)) * c + (*locat_x + j - 3)))
                {
                    //debug
                    //*(source + (r + 2 - (*locat_y + i)) * c + (*locat_x + j - 3))=false;
                    return false;
                }
            }
        }
    }
    return true;
}
bool game_core::Can_move_right(int *locat_x, int *locat_y, model *target)
{
    //从模型最右边起检索
    if (*locat_x + target->length == c + 2)
    {
        return false;
    }
    for (int j = target->length - 1; j >= 0; j--)
    {
        for (int i = target->height - 1; i >= 0; i--)
        {
            if (target->temp[i][j])
            {
                if (*(source + (r + 1 - (*locat_y + i)) * c + (*locat_x + j - 1)))
                {
                    //debug
                    //*(source + (r + 2 - (*locat_y + i)) * c + (*locat_x + j - 3))=false;
                    return false;
                }
            }
        }
    }
    return true;
}
void game_core::Write_core(int x, int y, model *target)
{
    for (int i = target->height - 1; i >= 0; i--)
    {
        for (int j = 0; j < target->length; j++)
        {
            if (target->temp[i][j])
            {
                *(source + (r + 1 - (y + i)) * c + (x + j - 2)) = true;
            }
        }
    }
}
bool game_core::Is_valid(int *x, int *y, model *target)
{
    //超出右边界不行
    if (*x + target->length > c + 2)
    {
        return false;
    }
    //超出下边界不行
    if (*y + target->height > r + 1)
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
                if (*(source + (r + 1 - (*y + i)) * c + (*x + j - 2)))
                {
                    return false;
                }
            }
        }
    }
    return true;
}