#include "model.hpp"
#include "keydec.hpp"
#define Print_base "▇"
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
    int r;
    int c;
    int speed;
    //bool over;
    clean_base *temp;
    bool model_written;
    //返回需要清除的行的位置
public:
    bool over;
    //删除临时结果
    void del_base();
    //调试使用
    void debug_core();
    //输出当前表
    void print();
    //初始化,speed为模型下落速度
    game_core(int r, int c ,int speed);
    ~game_core();
    //删除表中现有空行---辅助函数
    void flash();
    //删除clean所返回的需要删除的行--辅助函数
    void R_delete();
    //执行一次，满行全部删除
    int clean();
    //返回最小空行
    int Min_R();
    //添加模型
    void Add_model(model *target,Key_dec *Key);
    void Write_core(int x,int y,model *target);
    bool Can_move_down(int *x,int *y,model *target);
    bool Can_move_left(int *x,int *y,model *target);
    bool Can_move_right(int *x ,int *y,model *target);
};