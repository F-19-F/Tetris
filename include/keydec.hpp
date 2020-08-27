#ifndef _WIN32
#define sp1 27
#define sp2 91
#define up 65
#define down 66
#define right 67
#define left 68
#define space 32
#else
#define sp1 224
#define up 72
#define down 80
#define left 75
#define right 77
#define space 32
#endif
#include <queue>
//定义一个按键缓冲区，getkey得到的按键值存储在其中，方块下落时从中取出数据来响应
class Key_dec
{
public:
    Key_dec();
    int psignal;
    void start();
    void stop();
    int push(int signal);
    int pop();
    void clean();
};