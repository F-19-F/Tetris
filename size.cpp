#include "head.h"
void Size_deceter(bool* changed)
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