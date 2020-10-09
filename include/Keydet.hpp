#ifndef _KEY
#define _KEY
#ifndef _WIN32
#define sp1 27
#define sp2 91
#define up 65
#define down 66
#define right 67
#define left 68
#define space 32
#define enter 10
#else
#define sp1 224
#define up 72
#define down 80
#define left 75
#define right 77
#define space 32
#define enter 13
#endif
#define esc 27
class Key_det
{
public:
    Key_det();
    int psignal;
    void start();
    void stop();
    int push(int signal);
    int pop();
    void clean();
    void MutexLock(bool lock);
};
#endif