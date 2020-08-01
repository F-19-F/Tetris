#define sp1 27
#define sp2 91
#define up 65
#define down 66
#define right 67
#define left 68
//定义一个按键缓冲区，getkey得到的按键值存储在其中，方块下落时从中取出数据来响应
struct key_base
{
    int signal;
    key_base *next;
};
class Key_dec
{
private:
    key_base *first;
    key_base *last;

public:
    int psignal;
    Key_dec();
    ~Key_dec();
    void start();
    void stop();
    int push(int signal);
    int pop();
    void clean();
};