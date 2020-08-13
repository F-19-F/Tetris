#include <thread>
#include "include/size.hpp"
#ifndef _WIN64
#include <unistd.h>//linux/unix
#include <sys/ioctl.h>//linux/unix

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
#else
#include <windows.h>
size Getsize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    size a;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    a.c = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    a.r = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return a;
}
#endif