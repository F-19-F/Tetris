#ifndef _TUI
#define _TUI
#include "Keydec.hpp"
#include "Size.hpp"
#ifdef _WIN32
#define Title                              \
	cursor_move(x, y);                     \
	cout << " _____    _        _";        \
	movedown(1);                           \
	cout << "|_   _|__| |_ _ __(_)___";    \
	movedown(1);                           \
	cout << "  | |/ _ \\ __| '__| / __|";  \
	movedown(1);                           \
	cout << "  | |  __/ |_| |  | \\__ \\"; \
	movedown(1);                           \
	cout << "  |_|\\___|\\__|_|  |_|___/"; \
	movedown(1);
#else
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
#endif
extern Size _Gsize;
class Tetris_UI
{
private:
	Tetris_Core *Core;
	Key_dec *Key;
	Size Gsize;
	Size Win_Size;
public:
	Tetris_UI(Size Windows,Size Gsize ,Key_dec *key);
	int Dialog(char *content1,char *content2);
	int Menu();
	int Setting();
	int Start();
	int Over();
	int Infor(model *next_model);
};

/*int Over(int x, int y, int Score, Key_dec *Key);
int Setting(int x, int y, Key_dec *Key);
int Menu(int x, int y, Key_dec *Key);
void Infor_print(int x, int y, Tetris_Core *core, model *next_model);
int Startgame(int x, int y, Size Gsize, Key_dec *Key);*/
#endif