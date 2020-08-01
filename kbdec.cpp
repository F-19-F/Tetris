#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "include/keydec.h"
#include "include/model.h"
using namespace std;
Key_dec::Key_dec()
{
  first = last = new key_base;
  first->next = NULL;
  psignal=1;
}
Key_dec::~Key_dec()
{
  key_base *temp, *next_temp;
  temp = first;
  while (temp)
  {
    next_temp = temp->next;
    delete temp;
    temp = next_temp;
  }
}
int Key_dec::push(int signal)
{
  key_base *temp = new key_base;
  if (!temp)
  {
    //内存分配错误
    return -1;
  }
  temp->signal = signal;
  temp->next = NULL;
  last->next = temp;
  last = temp;
  return 0;
}
int Key_dec::pop()
{
  int r;
  //key_base as_return;
  if (first == last)
  {
    //as_return.signal = 0;
    //队列为空时返回空数据
    return 0;
  }
  key_base *temp = first->next;
  r=temp->signal;
  //as_return = *temp;
  //as_return.next = NULL;
  if (first->next == last)
  {
    last = first;
    first->next = NULL;
  }
  else
  {
    first->next = temp->next;
  }
  delete temp;
  return r;
}
void key_proc(bool ctrl, Key_dec *output)
{
  mutex c_lock;
  char c;
  static struct termios oldt, newt;
  static bool run = true;
  if (ctrl)
  {
    while (1)
    {
      //控制按键发现线程
      if (output->psignal == 0)
      {
        //cout << "end";
        //cout.flush();
        c_lock.lock();
        run = false;
        c_lock.unlock();
        return;
      }
      this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  }
  else
  {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt; //复制旧终端信息
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    system("stty -echo");
    while (run)
    {
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
            output->push(up);
            break;
          case down:
            //cout<<"Down"<<endl;
            output->push(down);
            break;
          case right:
            //cout<<"Right"<<endl;
            output->push(right);
            break;
          case left:
            //cout<<"Left"<<endl;
            output->push(left);
            break;
          default:
            break;
          }
        }
      }
    }
    system("stty echo"); //系统调用，恢复回显
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    c_lock.lock();
    run=true;
    c_lock.unlock();
  }
}
void Key_dec::start()
{
  thread t1(key_proc, true, this);
  thread t2(key_proc, false, this);
  t2.detach();
  t1.detach();
  //cin >> psignal;
}
void Key_dec::stop()
{
  psignal=0;
}
void Key_dec::clean()
{
  while (pop());
}