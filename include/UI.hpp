#ifndef _TUI
#define _TUI
#include "Keydet.hpp"
#include "Size.hpp"
#include "Tetris_Core.hpp"
#define About "这是一个C++俄罗斯方块游戏\n具有以下特征\n   1.游戏运行过程中途退出再次打开自动恢复\n   2.大小自适应\n   3.实时按键反馈\
\n \n By SWU-F19"
#define Warning "    检测到非正常结束\n    是否恢复上次游戏?\n"
#define HowToPlay "按方向键盘'←'和'→'来移动方块\n按'↑'来改变形状\n按'↓'来加速下落\n"
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
#define Act_Change            \
	if (size_changed)         \
	{                         \
		hide_cursor();        \
		UpdateSize();         \
		size_changed = false; \
		goto Sudden_Changed;  \
	}
#define Work_XY(offset_x, offset_y)    \
	int x = Win_Size.c / 2 + offset_x; \
	int y = offset_y;
#define Update_XY(offset_x, offset_y) \
	x = Win_Size.c / 2 + offset_x;    \
	y = offset_y;
extern Size _Gsize;
class Tetris_UI
{
private:
	Tetris_Core *Core;
	Key_det *Key;
	Size Gsize;
	Settings cur_config;
public:
	Size Win_Size;
	bool size_changed;
	model *NEXT;
	Tetris_UI(Size Windows,Size Gsize ,Key_det *key);
	~Tetris_UI();
	int UpdateSize();
	int Dialog(char *option1,char *option2 ,char* TITLE,char* content);
	int Menu();
	int Setting();
	int Start(bool Reco);
	int Over();
	int Infor();
	int Infor(bool Pause);
};
extern Tetris_UI *_UI;//一个全局的的UI对象指针，方便调用
#endif