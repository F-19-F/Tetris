#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <memory.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define Print_base "▇"
using namespace std;
void Size_deceter(bool* changed);
struct clean_base
{
    int location;
    clean_base *next;
};
class game_core
{
    //权限设置为private保护游戏核心数据安全
private:
    bool *source;
    int x;
    int y;
    clean_base *temp;
    //返回需要清除的行的位置
public:
    //删除临时结果
    void del_base();
    //调试使用
    void debug_core();
    //输出当前表
    void print();
    //初始化
    game_core(int x, int y);
    ~game_core();
    //删除表中现有空行---辅助函数
    void flash();
    //删除clean所返回的需要删除的行--辅助函数
    void R_delete();
    //执行一次，满行全部删除
    int clean();
    int Min_R();
};
class model
{
    private:
    //方块抽象为8X8方格上相隔1个方块的点的组合
    bool base[8][8];
    //下面为旋转中心的坐标
    int center_x;
    int center_y;
    public:
    //mode表示方块类型，1-5将构建预设类型，0代表自主构建
    model(int mode);
    //输出原始数据，以便调试--for debug
    void print_row();
    //输出model形状
    void print_model();
    //以旋转中心逆时针旋转90度
    void changer_neg();
};
//这是头文件包含目录，所有的头文件应该在这里include