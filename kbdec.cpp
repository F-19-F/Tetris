#include <termios.h>
#include <unistd.h>
#include <iostream>
#include "include/keydec.h"
using namespace std;
int getkey(int* output)
{
  char c;
  /*Information below can be found in https://man7.org/linux/man-pages/man3/termios.3.html*/
  /*This function works both on Macos and Linux */
  /*This file was analysed by F-19-F*/
  static struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);//意为获取旧终端的某些信息
  /*gets the parameters associated with the object referred
    by fd and stores them in the termios structure referenced by
    termios_p.  This function may be invoked from a background process;
    however, the terminal attributes may be subsequently changed by a
    foreground process.*/
  newt = oldt;//复制旧终端信息
  newt.c_lflag &= ~(ICANON);//对旧的终端信息中的local modes进行修改，实现直接输入的关键
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);//应用修改后的终端信息，以实现不回车输入
  /*sets the parameters associated with the terminal 
  (unlesssupport is required from the underlying hardware that is not avail‐able) 
  from the termios structure referred to by termios_p.optional_actions specifies 
  when the changes take effect:*/
  //TCSANOW: the change occurs immediately.
  system("stty -echo"); //系统调用，禁止回显
  while ((c = getchar()) != 'e')//设置输入e时退出
  {
    //char d = c + 'A' - 'a'; //将字符转换成大写
    //cout << (int)c << endl;
    if ((int)c==sp1)
    {
      c=getchar();
      if ((int)c==sp2)
      {
        c=getchar();
        switch (c)
        {
        case up:
          //cout <<"Up"<<endl;
          *output=up;
          break;
        case down:
         //cout<<"Down"<<endl;
         *output=down;
         break;
        case right:
          //cout<<"Right"<<endl;
          *output=right;
        break;
        case left:
          //cout<<"Left"<<endl;
          *output=left;
        break;
        default:
          break;
        }
      }
    }
  }
  system("stty echo"); //系统调用，恢复回显
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);//恢复终端模式
  return 0;
}