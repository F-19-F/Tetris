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