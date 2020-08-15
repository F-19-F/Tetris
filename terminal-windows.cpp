#ifdef _WIN32
#include "windows.h"
#include "include/WinAPI_control.hpp"
COORD common;
//记录每一次cout导致的光标位移
int cursor_location=0;
bool OpenANSIControlChar()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) { return false; }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) { return false; }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) { return false; }
    return true;
}
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
void dishide_cusor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO Cursor;
    GetConsoleCursorInfo(handle, &Cursor); //获取控制台光标信息
    Cursor.bVisible = true;               //设置属性
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
void yellow_foreground()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN);
}
void yellow_background()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN|BACKGROUND_RED);
}
void end_all()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
}
#endif