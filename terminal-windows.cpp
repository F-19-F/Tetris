#ifdef _WIN32
#include "windows.h"
#include "include/WinAPI_control.hpp"
#include "include/model.hpp"
COORD common;
//记录每一次cout导致的光标位移
int cursor_location=0;
COLORREF ColorTable_Back[16];
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
void Set_Default_color(int r,int g,int b,int f_r,int f_g,int f_b)
{
    int Temp[6][3]={Random_Color_1,Random_Color_2,Random_Color_3,Random_Color_4,Random_Color_5,Random_Color_6};
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    //Win10上，ColorTable 共有16个元素 (取自wincon.h中对应结构体的定义)
    //在修改颜色前备份对应颜色
    for (int i=0;i<16;i++)
    {
        ColorTable_Back[i]=info.ColorTable[i];
    }
    info.ColorTable[0]=RGB(r,g,b);//0是默认的背景RGB颜色
    for (int i=0;i<6;i++)
    {
        info.ColorTable[i+1]=RGB(Temp[i][0],Temp[i][1],Temp[i][2]);
    }
    info.ColorTable[7]=RGB(f_r,f_g,f_b);//7 是默认的文字RGB颜色

    SetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
void color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
	return;
}
void Reset_color()
{
    //Set_Default_color(12,12,12,204,204,204);//在Win10，背景的RGB默认值是12,12,12 字体的RGB默认值是204,204,204
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    //恢复默认值
    GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    for (int i=0;i<8;i++)
    {
        info.ColorTable[i]=ColorTable_Back[i];
    }
    SetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    color(7);//在Win10,字体默认的color为7
    return;
}
#endif