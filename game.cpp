#include "head.h"
#include "terminal-linux.h"
bool changed = false;
void size()
{
    Size_deceter(changed);
}
int main()
{
    //test
    //程序输出有两种方式：一种是即时处理方式，另一种是先暂存起来，然后再大块写入的方式，前者往往造成较高的系统负担，实测关闭buf的cpu占用是开启的一倍
    //故没有采用直接关闭buf
    game_core *test;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    test = new game_core(w.ws_row, (w.ws_col / 3) * 2);
    thread t1(size);
    t1.detach();
    while(1)
    {
        test->debug_core();
        test->print();
        this_thread::sleep_for(std::chrono::milliseconds(100));//c++特有的休眠方式
        test->clean();
        test->print();
        this_thread::sleep_for(std::chrono::milliseconds(200));//c++特有的休眠方式
        if (changed)
        {
            clean_screen();
            cout.flush();
            delete test;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            test = new game_core(w.ws_row, (w.ws_col / 3) * 2);
            changed = false;
        }
    }
    //row代表能打印多少行bool changed = false;
    getchar();
    cout << endl;
    return 0;
}