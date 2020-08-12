#ifdef _WIN64
#include "windows.h"
#include "include/terminal-windows.hpp"
COORD common;
//记录每一次cout导致的光标位移
int cursor_location=0;
void cursor_move(int x, int y)
{
    //COORD common;
    common.X = x;
    common.Y = y;
    //重新设置光标位置后将位移重置为0
    cursor_location=0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), common);
}
void hide_cursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO Cursor;
    GetConsoleCursorInfo(handle, &Cursor); //获取控制台光标信息
    Cursor.bVisible = false;               //设置属性
    SetConsoleCursorInfo(handle, &Cursor); //使更改后的属性生效
}
void moveright(int i)
{
    //COORD common;
    common.X = common.X + cursor_location + i;
    cursor_location=0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), common);
}
void moveleft(int i)
{
    //COORD common;
    common.X=common.X + cursor_location - i;
    cursor_location=0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), common);
}
void moveup(int i)
{
    //COORD common;
    common.X=common.X+cursor_location;
    cursor_location=0;
    common.Y = common.Y - i;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), common);
}
void movedown(int i)
{
    //下移时重置为0
    common.X=common.X+cursor_location;
    cursor_location=0;
    common.Y = common.Y + i;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), common);
}
void clean_screen()
{
    system("cls");
}
void blue_foreground()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_INTENSITY);
}
void end_all()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
}
#endif