#ifndef _TUI
#define _TUI
#include "Keydec.hpp"
#include "Size.hpp"
#define About "这是一个C++俄罗斯方块游戏\n具有以下特征\n   1.游戏运行过程中途退出再次打开自动恢复\n   2.大小自适应\n   3.实时按键反馈\
\n \n By SWU-F19"
#define Warning "    检测到非正常结束\n    是否恢复上次游戏?\n"
#define Win_Char_Buffer_Size 100
#define Title                              \
	cursor_move(x, y);                     \
	cout << " _____    _        _";        \
	cursor_move(x, y);                     \
	movedown(1);                           \
	cout << "|_   _|__| |_ _ __(_)___";    \
	cursor_move(x, y);                     \
	movedown(2);                           \
	cout << "  | |/ _ \\ __| '__| / __|";  \
	cursor_move(x, y);                     \
	movedown(3);                           \
	cout << "  | |  __/ |_| |  | \\__ \\"; \
	cursor_move(x, y);                     \
	movedown(4);                           \
	cout << "  |_|\\___|\\__|_|  |_|___/"; \
	cursor_move(x, y);                     \
	movedown(5);
extern Size _Gsize;
class Tetris_UI
{
private:
	Tetris_Core *Core;
	Key_dec *Key;
	Size Gsize;
	Size Win_Size;
	
public:
	bool size_changed;
	Tetris_UI(Size Windows,Size Gsize ,Key_dec *key);
	~Tetris_UI();
	int UpdateSize();
	int Dialog(char *option1,char *option2 ,char* TITLE,char* content);
	int Menu();
	int Setting();
	int Start();
	int Over();
	int Infor(model *next_model);
};
#endif