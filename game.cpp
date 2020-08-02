#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/ioctl.h>              //linux/unix
#include "include/terminal-linux.h" //linux/unix
#include "include/game_core.h"
#include "include/size.h"
using namespace std;
int main()
{
    Key_dec Key;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    model *a;
    Key.start();
    game_core b(w.ws_row,(int)((w.ws_col)/3),1);
    b.debug_core();
    for (int i=1;i<=5;i++)
    {
        a=new model(i);
        b.Add_model(a,&Key);
    }
    Key.stop();
    return 0;
}