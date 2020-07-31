#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include "include/keydec.h"
#include "include/model.h"
#include "include/terminal-linux.h"
using namespace std;
//output指定哪个按键按下了，times指明按了多少次，reset为1时将重置,为0时将运行，为-1时将退出
int getkey(int *output, int *times, int *reset)
{
  char c;
  mutex v_lock;
  //用来记录上次按下的按键
  char temp = 0;
  /*Information below can be found in https://man7.org/linux/man-pages/man3/termios.3.html*/
  /*This function works both on Macos and Linux */
  /*This file was analysed by F-19-F*/
  static struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt); //意为获取旧终端的某些信息
  /*gets the parameters associated with the object referred
    by fd and stores them in the termios structure referenced by
    termios_p.  This function may be invoked from a background process;
    however, the terminal attributes may be subsequently changed by a
    foreground process.*/
  newt = oldt;                             //复制旧终端信息
  newt.c_lflag &= ~(ICANON);               //对旧的终端信息中的local modes进行修改，实现直接输入的关键
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); //应用修改后的终端信息，以实现不回车输入
  /*sets the parameters associated with the terminal 
  (unlesssupport is required from the underlying hardware that is not avail‐able) 
  from the termios structure referred to by termios_p.optional_actions specifies 
  when the changes take effect:*/
  //TCSANOW: the change occurs immediately.
  system("stty -echo"); //系统调用，禁止回显
  while (1)             //设置输入e时退出
  {
    //char d = c + 'A' - 'a'; //将字符转换成大写
    //cout << (int)c << endl;
    if ((*reset) == 1)
    {
      *times = 0;
      *output = 1;
      *reset = 0; //修改后重置reset，避免问题
      //system("stty echo");
      //tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      //return 0;
    }
    if ((*reset) == -1)
    {
      system("stty echo");
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      return 0;
    }
    c = getchar();
    if ((int)c == sp1)
    {
      c = getchar();
      if ((int)c == sp2)
      {
        c = getchar();
        switch (c)
        {
        case up:
          //cout <<"Up"<<endl;
          if (c == temp)
          {
            *times = *times + 1;
          }
          else
          {
            *times = 1;
          }
          *output = up;
          temp = c;
          break;
        case down:
          //cout<<"Down"<<endl;
          if (c == temp)
          {
            *times = *times + 1;
          }
          else
          {
            *times = 1;
          }
          *output = down;
          temp = c;
          break;
        case right:
          //cout<<"Right"<<endl;
          if (c == temp)
          {
            *times = *times + 1;
          }
          else
          {
            *times = 1;
          }
          *output = right;
          temp = c;
          break;
        case left:
          //cout<<"Left"<<endl;
          if (c == temp)
          {
            *times = *times + 1;
          }
          else
          {
            *times = 1;
          }
          *output = left;
          temp = c;
          break;
        default:
          break;
        }
      }
    }
    //this_thread::sleep_for(std::chrono::milliseconds(200)); //c++特有的休眠方式
  }
  system("stty echo");                     //系统调用，恢复回显
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //恢复终端模式
  return 0;
}
int Getkey(model *target, bool *signal, int *x, int *y)
{
  int c;
  /*Information below can be found in https://man7.org/linux/man-pages/man3/termios.3.html*/
  /*This function works both on Macos and Linux */
  /*This file was analysed by F-19-F*/
  static struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt); //意为获取旧终端的某些信息
  /*gets the parameters associated with the object referred
    by fd and stores them in the termios structure referenced by
    termios_p.  This function may be invoked from a background process;
    however, the terminal attributes may be subsequently changed by a
    foreground process.*/
  newt = oldt;                             //复制旧终端信息
  newt.c_lflag &= ~(ICANON);               //对旧的终端信息中的local modes进行修改，实现直接输入的关键
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); //应用修改后的终端信息，以实现不回车输入
  /*sets the parameters associated with the terminal 
  (unlesssupport is required from the underlying hardware that is not avail‐able) 
  from the termios structure referred to by termios_p.optional_actions specifies 
  when the changes take effect:*/
  //TCSANOW: the change occurs immediately.
  system("stty -echo"); //系统调用，禁止回显
  while (*signal)       //设置输入e时退出
  {
    //char d = c + 'A' - 'a'; //将字符转换成大写
    //cout << (int)c << endl;
    /*if ((*signal)==-1)
    {
      system("stty echo");
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      return 0;
    }*/
    c = (int)getchar();
    if ((int)c == sp1)
    {
      c = (int)getchar();
      if ((int)c == sp2)
      {
        c = (int)getchar();
        switch (c)
        {
        case up:
          //cout <<"Up"<<endl;
          break;
        case down:
          //cout<<"Down"<<endl;
          break;
        case right:
          //cout<<"Right"<<endl;
          break;
        case left:
          //删除当前打印的方块

          
          //更改地址
          if (*x - 1 > 2)
          {
            cursor_move(*x, *y);
            target->print_model(true);
            *x = *x - 1;
            cursor_move(*x, *y);
            //this_thread::sleep_for(std::chrono::milliseconds(50));
            target->print_model(false);
            cursor_move(*x, *y);
          }

          //cout<<"Left"<<endl;
          break;
        default:
          break;
        }
      }
    }
    this_thread::sleep_for(std::chrono::milliseconds(100));
    //this_thread::sleep_for(std::chrono::milliseconds(200)); //c++特有的休眠方式
  }
  system("stty echo");                     //系统调用，恢复回显
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //恢复终端模式
  return 0;
}