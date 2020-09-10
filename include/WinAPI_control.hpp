#ifndef _WIN_C
#define _WIN_C
void Win_Required();
void Reset_Win_Required();
void cursor_move(int x, int y);
void hide_cursor();
void dishide_cusor();
void moveright(int i);
void moveleft(int i);
void movedown(int i);
void moveup(int i);
void clean_screen();
void Set_Default_color(int r, int g, int b, int f_r, int f_g, int f_b);
void color(int c);
void Reset_color();
void Hide_File(char* path);
//请确保在调用cursormove后任意的输出都记录下来
extern int cursor_location;
#endif