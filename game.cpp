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
    //test
    //程序输出有两种方式：一种是即时处理方式，另一种是先暂存起来，然后再大块写入的方式，前者往往造成较高的系统负担，实测关闭buf的cpu占用是开启的一倍
    //故没有采用直接关闭buf
/*int main()
{
    int key_signal=0;
    int Press_times=0;
    //int key_run=-1;
    //std::thread t1(getkey,&key_signal,&Press_times,&key_run);
    //t1.detach();
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    game_core a(w.ws_row, (w.ws_col / 3) * 2,1);
    model *b;
    for (int i=1;i<=5;i++)
    {
        //a.debug_core();
        //a.clean();
        //key_run=0;
        b=new model(2);
        a.Add_model(b,&key_signal,&Press_times);
        //Press_times=0;
        delete b;
        //key_lock.lock();
        //key_run=1;
        //Press_times=0;
        //key_lock.unlock();
    }
    //model b(5);
    //a.Add_model(&b);
    cout<<endl;
   // key_run=-1;
    /**********直接getchar()行不通
    while (1)
    {
        std::cout<<getchar();
    }
    
    //std::thread t1(ky);
    //t1.detach();
    //t1.~thread();
    return 0;
}*/
int main()
{
    model* b;
    int j=4;
    for (int i=1;i<=5;i++)
    {
        b= new model(i);
        do
        {
            b->get_temp();
            b->print_row(1);
            moveright(10);
            cout<<"height"<<b->height<<"length"<<b->length;
            b->changer_neg(90);
            cout<<endl;
            this_thread::sleep_for(std::chrono::milliseconds(200));

        } while (--j);
        j=4;
        delete b;
    }
    return 0;
}
