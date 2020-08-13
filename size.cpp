#ifndef _WIN64
#include <unistd.h>//linux/unix
#include <sys/ioctl.h>//linux/unix
#include <thread>
#include "include/size.hpp"
void Size_detecter(bool* changed)
{
    int ini_hight = 0;
    int ini_long = 0;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ini_hight = w.ws_row;
    ini_long = w.ws_col;
    while (1)
    {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        if (ini_hight != w.ws_row || ini_long != w.ws_col)
        {
            *changed = true;
            ini_hight = w.ws_row;
            ini_long = w.ws_col;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
    }
}
size Getsize()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    size a;
    a.r=w.ws_row;
    a.c=w.ws_col;
    return a;
}
#endif