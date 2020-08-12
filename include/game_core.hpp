#include "model.hpp"
#include "keydec.hpp"
#define MAX_TIME 1000
//#define Print_base "■"
#define Print_base "#"
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
    clean_base *temp;
public:
    bool over;
    void del_base();
    void debug_core();
    void print();
    game_core(int r, int c ,int speed);
    ~game_core();
    void flash();
    void R_delete();
    int clean();
    int Min_R();
    void draw_delline();
    void Add_model(model *target,Key_dec *Key);
    void Write_core(int x,int y,model *target);
    bool Can_move_down(int *x,int *y,model *target);
    bool Can_move_left(int *x,int *y,model *target);
    bool Can_move_right(int *x ,int *y,model *target);
    bool Is_valid(int *x,int *y,model *target);
};