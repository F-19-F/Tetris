#include "Model.hpp"
#include "Keydec.hpp"
#define MAX_TIME 1000
#define BF_Default_Color_RGB 0, 0, 0, 158, 173, 134
//由于Windows的坐标机制不一样，故分出两种初始偏移量
#ifdef _WIN32
#define x_ini_offset -1
#define y_ini_offset -1
#else
#define x_ini_offset 0
#define y_ini_offset 0
extern bool compat_mode;
void SetColorCompat(bool opt);
#endif
struct clean_base
{
    int location;
    clean_base *next;
};
class Tetris_Core
{
private:
    bool *source;
    int *Color;
    int r;
    int c;
    int speed;
    int score;
    int x_offset;
    int y_offset;
    clean_base *temp;
    void Clean_base_cache();
    void Hide_Empty_Line();
    void R_delete();
    void Del_SE();
    int Min_R();
    void Write_core(int x, int y, model *target);

public:
    bool over;
    void Core_Print();
    int get_score();
    int get_speed();
    Tetris_Core(int r, int c, int x_offset, int y_offset, int speed);
    ~Tetris_Core();
    int Full_Line_Clean();
    void Add_model(model *target, Key_dec *Key);
    bool Can_move_down(int x, int y, model *target);
    bool Can_move_left(int x, int y, model *target);
    bool Can_move_right(int x, int y, model *target);
    bool Is_valid(int x, int y, model *target);
};