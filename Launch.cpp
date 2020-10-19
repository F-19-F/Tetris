/*
main入口
*/
#include <iostream>
#include <thread>
#ifndef _WIN32
#include "include/ANSI_control.hpp" //linux/unix
#else
#include "include/WinAPI_control.hpp"
#endif
#include "include/Tetris_Core.hpp"
#include "include/Size.hpp"
#include "include/UI.hpp"
using namespace std;
Size _Gsize;
int main()
{
	//windows需要转换编码，以解决显示问题
#ifdef _WIN32
	Win_Required();
#endif
	hide_cursor();
	Set_Default_color(BF_Default_Color_RGB);
	Size Win_size = Getsize(); //获取窗口大小
	_Gsize.r = 27;
	_Gsize.c = 18;
	Key_det Key;
	Tetris_UI UI(Win_size, _Gsize, &Key);
	int Opt = 0;
	Key.start();
	while ((Opt = UI.Menu()) != 4)
	{
		switch (Opt)
		{
		case 0:
			UI.Start(false);//启动一个新游戏，Menu会自动判断是否非正常关闭
			break;
		case 1:
			UI.Setting();
			break;
		case 2:
			UI.Dialog((char *)"确认", NULL, (char *)"玩法", (char *)HowToPlay);
			break;
		case 3:
			UI.Dialog((char *)"确认", NULL, (char *)"关于", (char *)About);
			break;
		default:
			break;
		}
	}
	//重置终端颜色
	Reset_color();
	clean_screen();
	cout << "游戏已结束，按任意键退出游戏";
	cout.flush();
#ifdef _WIN32
	Reset_Win_Required();
#endif
	Key.stop();
	//运行完后恢复光标显示
	dishide_cusor();
	return 0;
}