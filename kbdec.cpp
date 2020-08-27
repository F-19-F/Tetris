#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#else
#include <conio.h>
#endif
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>
#include "include/keydec.hpp"
#include "include/model.hpp"
using namespace std;
queue<int> base;
mutex Queue_lock;
Key_dec::Key_dec()
{
  psignal = 1;
}
int Key_dec::push(int signal)
{
  base.push(signal);
  return 0;
}
int Key_dec::pop()
{
  int r;
  if (base.empty())
  {
    //队列为空时返回0
    return 0;
  }
  r = base.front();
  base.pop();
  return r;
}
#ifndef _WIN32
void key_proc(bool ctrl, Key_dec *output)
{
  char c;
  static struct termios oldt, newt;
  static bool run = true;
  static bool changed = false;
  if (ctrl)
  {
    while (1)
    {
      //控制按键发现线程
      if (output->psignal == 0)
      {
        run = false;
        return;
      }
      this_thread::sleep_for(std::chrono::milliseconds(20));
    }
  }
  else
  {
    if (!changed)
    {
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt; //复制旧终端信息
      newt.c_lflag &= ~(ICANON);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      system("stty -echo");
      changed = true;
    }
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
            Queue_lock.lock();
            output->push(up);
            Queue_lock.unlock();
            break;
          case down:
            Queue_lock.lock();
            output->push(down);
            Queue_lock.unlock();
            break;
          case right:
            Queue_lock.lock();
            output->push(right);
            Queue_lock.unlock();
            break;
          case left:
            Queue_lock.lock();
            output->push(left);
            Queue_lock.unlock();
            break;
          default:
            break;
          }
        }
      }
      else if (c == space)
      {
        output->push(space);
      }
      else
      {
        if (!run)
        {
          output->push(-1);
        }
      }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    system("stty echo 2>/dev/null"); //系统调用，恢复回显
    changed = false;
    run = true;
    return;
  }
}
#else
void key_proc(bool ctrl, Key_dec *output)
{
  int c;
  static bool run = true;
  if (ctrl)
  {
    while (1)
    {
      //控制按键发现线程
      if (output->psignal == 0)
      {
        run = false;
        return;
      }
      this_thread::sleep_for(std::chrono::milliseconds(20));
    }
  }
  else
  {
    while (run)
    {
      c = getch();
      if (sp1 == (int)c)
      {
        c = getch();
        switch (c)
        {
        case up:
          Queue_lock.lock();
          output->push(up);
          Queue_lock.unlock();
          break;
        case down:
          Queue_lock.lock();
          output->push(down);
          Queue_lock.unlock();
          break;
        case right:
          Queue_lock.lock();
          output->push(right);
          Queue_lock.unlock();
          break;
        case left:
          Queue_lock.lock();
          output->push(left);
          Queue_lock.unlock();
          break;
        default:
          break;
        }
      }
      else if (c == space)
      {
        Queue_lock.lock();
        output->push(space);
        Queue_lock.unlock();
      }
      else
      {
        if (!run)
        {
          output->push(-1);
        }
      }
    }
    run = true;
  }
}
#endif
void Key_dec::start()
{
  thread t1(key_proc, true, this);
  thread t2(key_proc, false, this);
  t2.detach();
  t1.detach();
}
void Key_dec::stop()
{
  psignal = 0;
}
void Key_dec::clean()
{
  while (!base.empty())
  {
    base.pop();
  }
}
void Key_dec::MutexLock(bool lock)
{
  if (lock)
  {
    Queue_lock.lock();
  }
  else
  {
    Queue_lock.unlock();
  }
}