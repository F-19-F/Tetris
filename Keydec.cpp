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
#include "include/Keydec.hpp"
#include "include/Model.hpp"
using namespace std;
queue<int> base;
mutex Queue_lock;
mutex Run_lock;
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
void key_proc(mutex *ctrl, Key_dec *output)
{
  char c;
  static struct termios oldt, newt;
  static bool changed = false;
  if (!changed)
  {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt; //复制旧终端信息
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    system("stty -echo");
    changed = true;
  }
  while (ctrl->try_lock())
  {
    ctrl->unlock();
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
      Queue_lock.lock();
      output->push(space);
      Queue_lock.unlock();
    }
    else
    {
      if (!ctrl->try_lock())
      {
        output->psignal = -1;
        return;
      }
      else
      {
        ctrl->unlock();
      }
    }
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  system("stty echo 2>/dev/null"); //系统调用，恢复回显
  changed = false;
  output->psignal = -1;
  return;
}
#else
void key_proc(mutex *ctrl, Key_dec *output)
{
  int c;
  while (ctrl->try_lock())
  {
    ctrl->unlock();
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
      if (!ctrl->try_lock())
      {
        output->psignal = -1;
        return;
      }
      else
      {
        ctrl->unlock();
      }
    }
  }
  output->psignal = -1;
  return;
}
#endif
void Key_dec::start()
{
  thread t1(key_proc, &Run_lock, this);
  t1.detach();
}
void Key_dec::stop()
{
  Run_lock.lock();
  while (psignal != -1)
  {
    this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  Run_lock.unlock();
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