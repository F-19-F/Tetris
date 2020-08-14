#include "model.hpp"
#include "keydec.hpp"
#define MAX_TIME 1000
//#define Print_base "■"
#define Print_base "#"
#define x_offset -1
#define y_offset -1
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
    int score;
    clean_base *temp;
    void del_base();
    void flash();
    void R_delete();
    void draw_delline();
    int Min_R();
void Write_core(int x, int y, model *target);
public:
    bool over;
    void debug_core();
    void print();
    int get_score();
    game_core(int r, int c, int speed);
    ~game_core();
    int clean();
    void Add_model(model *target, Key_dec *Key);
    bool Can_move_down(int x, int y, model *target);
    bool Can_move_left(int x, int y, model *target);
    bool Can_move_right(int x, int y, model *target);
    bool Is_valid(int x, int y, model *target);
};