#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/ioctl.h>//linux/unix
#include "include/terminal-linux.h"//linux/unix
#include "include/game_core.h"
#include "include/size.h"
#include "include/keydec.h"
bool changed = false;
using namespace std;
void size()
{
    Size_detecter(&changed);
}
/*void ky()
{
    getkey(key_signal);
}*/
/*int main()
{
    //test
    //程序输出有两种方式：一种是即时处理方式，另一种是先暂存起来，然后再大块写入的方式，前者往往造成较高的系统负担，实测关闭buf的cpu占用是开启的一倍
    //故没有采用直接关闭buf
    game_core *test;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    test = new game_core(w.ws_row, (w.ws_col / 3) * 2,1);
    thread t1(size);
    t1.detach();
    while(1)
    {
        //test->debug_core();
        clean_screen();
        test->print();
        this_thread::sleep_for(std::chrono::milliseconds(1000));//c++特有的休眠方式
        test->clean();
        clean_screen();
        test->print();
        this_thread::sleep_for(std::chrono::milliseconds(2000));//c++特有的休眠方式
        if (changed)
        {
            clean_screen();
            cout.flush();
            delete test;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            test = new game_core(w.ws_row, (w.ws_col / 3) * 2,1);
            changed = false;
        }
    }
    //row代表能打印多少行bool changed = false;
    getchar();
    cout << endl;
    return 0;
}*/
mutex key_lock;
int main()
{
    int key_signal=0;
    int Press_times=0;
    int key_run=-1;
    std::thread t1(getkey,&key_signal,&Press_times,&key_run);
    t1.detach();
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    game_core a(w.ws_row, (w.ws_col / 3) * 2,1);
    model *b;
    for (int i=1;i<=5;i++)
    {
        //a.debug_core();
        //a.clean();
        key_run=0;
        b=new model(2);
        a.Add_model(b,&key_signal,&Press_times);
        //Press_times=0;
        delete b;
        key_lock.lock();
        //key_run=1;
        Press_times=0;
        key_lock.unlock();
    }
    //model b(5);
    //a.Add_model(&b);
    cout<<endl;
    key_run=-1;
    /**********直接getchar()行不通
    while (1)
    {
        std::cout<<getchar();
    }*/
    
    //std::thread t1(ky);
    //t1.detach();
    t1.~thread();
    return 0;
}