void cursor_move(int x,int y);
void hide_cursor();
void dishide_cusor();
void moveright(int i);
void moveleft(int i);
void movedown(int i);
void moveup(int i);
void clean_screen();
void blue_foreground();
void end_all();
//请确保在调用cursormove后任意的输出都记录下来
extern int cursor_location;