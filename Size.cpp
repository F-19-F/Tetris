/*这是窗口大小发现的实现*/
#include <thread>
#include <mutex>
#include "include/Size.hpp"
using namespace std;
mutex Sync_Lock;
#ifndef _WIN32
#include <unistd.h>    //linux/unix
#include <sys/ioctl.h> //linux/unix
Size Getsize()
{
    Sync_Lock.lock();
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    Size a;
    a.r = w.ws_row;
    a.c = w.ws_col;
    Sync_Lock.unlock();
    return a;
}
#else
#include <windows.h>
Size Getsize()
{
    Sync_Lock.lock();
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    Size a;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    a.c = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    a.r = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    Sync_Lock.unlock();
    return a;
}
#endif
void Size_detecter(bool *changed,mutex *Run_Lock)
{
    Size bak=Getsize();
    Size current;
    while (1)
    {
        if (Run_Lock->try_lock())
        {
            Run_Lock->unlock();
        }
        else
        {
            return;
        }
        current =Getsize();
        if (current.r!=bak.r||current.c!=bak.c)
        {
            bak=current;
            *changed=true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
}