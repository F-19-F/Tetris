#include <iostream>
#include <thread>
#ifndef _WIN32
#include "include/ANSI_control.hpp" //linux/unix
#else
#include "include/WinAPI_control.hpp"
#endif
#include "include/game_core.hpp"
#include "include/size.hpp"
using namespace std;
int First_flag = 0;//用于游戏和信息显示通信
int Menu(int x, int y, Key_dec *Key)
{
	clean_screen();
	int Cur_Location_Y = y + 7;
#ifdef _WIN32
	cursor_move(x, y);
	cout << " _____    _        _";
	movedown(1);
	cout << "|_   _|__| |_ _ __(_)___";
	movedown(1);
	cout << "  | |/ _ \\ __| '__| / __|";
	movedown(1);
	cout << "  | |  __/ |_| |  | \\__ \\";
	movedown(1);
	cout << "  |_|\\___|\\__|_|  |_|___/";
	movedown(1);
#else
	cursor_move(x, y);
	save_cursor();
	cout << " _____    _        _";
	restore_cursor();
	movedown(1);
	cout << "|_   _|__| |_ _ __(_)___";
	restore_cursor();
	movedown(1);
	cout << "  | |/ _ \\ __| '__| / __|";
	restore_cursor();
	movedown(1);
	cout << "  | |  __/ |_| |  | \\__ \\";
	restore_cursor();
	movedown(1);
	cout << "  |_|\\___|\\__|_|  |_|___/";
	restore_cursor();
	movedown(1);
#endif
	cursor_move(x + 8, y + 7);
	cout << "开始游戏";
	cursor_move(x + 8, y + 9);
	cout << "游戏设置";
	cursor_move(x + 8, y + 11);
	cout << "玩法介绍";
	cursor_move(x + 8, y + 13);
	cout << "关于游戏";
	cursor_move(x + 8, y + 15);
	cout << "退出游戏";
	cursor_move(x + 5, Cur_Location_Y);
	cout << "-->";
	cursor_move(x + 16, Cur_Location_Y);
	cout << "<--";
	while (1)
	{
		Key->MutexLock(true);
		switch (Key->pop())
		{
		case up:
			cursor_move(x + 5, Cur_Location_Y);
			cout << "   ";
			cursor_move(x + 16, Cur_Location_Y);
			cout << "   ";
			if (Cur_Location_Y == y + 7)
			{
				Cur_Location_Y = y + 15;
			}
			else
			{
				Cur_Location_Y -= 2;
			}
			cursor_move(x + 5, Cur_Location_Y);
			cout << "-->";
			cursor_move(x + 16, Cur_Location_Y);
			cout << "<--";
			break;
		case down:
			cursor_move(x + 5, Cur_Location_Y);
			cout << "   ";
			cursor_move(x + 16, Cur_Location_Y);
			cout << "   ";
			if (Cur_Location_Y == y + 15)
			{
				Cur_Location_Y = y + 7;
			}
			else
			{
				Cur_Location_Y += 2;
			}
			cursor_move(x + 5, Cur_Location_Y);
			cout << "-->";
			cursor_move(x + 16, Cur_Location_Y);
			cout << "<--";
			break;
		case space:
			Key->MutexLock(false);
			return ((Cur_Location_Y - y - 7) / 2);
		}
		Key->MutexLock(false);
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void Infor_print(int x, int y, game_core *core, model *next_model)
{

	static model Last_model = *next_model;
	if (First_flag != 0)
	{
		cursor_move(x, y + 7);
		Last_model.print_model(true);
	}
	color(7);
	cursor_move(x, y);
	cout << "Score:" << core->get_score();
	cout.flush();
	cursor_move(x, y + 3);
	cout << "Level:" << core->get_speed();
	cout.flush();
	cursor_move(x, y + 6);
	cout << "Next:";
	cout.flush();
	cursor_move(x, y + 7);
	next_model->print_model(false);
	Last_model = *next_model;
	First_flag = 1;
}
int Startgame(Key_dec *Key)
{
	int i;
	model *a;
	model *temp;
	Size ini_size;
	ini_size = Getsize();
	srand((unsigned)time(NULL));
	game_core b(ini_size.r - 3, (int)((ini_size.c) / 5), 1, 0, 10);
	clean_screen();
	cout.flush();
	b.print();
	i = rand() % 7 + 1;
	while (!b.over)
	{
		a = new model(i);
		i = rand() % 7 + 1;
		temp = new model(i);
		Infor_print(ini_size.c / 5 + 5, 3, &b, temp);
		b.Add_model(a, Key);
		delete a;
		delete temp;
		Key->clean();
		b.print();
	}
	cursor_move(ini_size.r, ini_size.c);
	//Reset_color();
	First_flag = 0;
	return b.get_score();
}
int main()
{
	//windows需要转换编码，以解决显示问题
#ifdef _WIN32
	Win_Required();
#endif
	hide_cursor();
	Set_Default_color(BF_Default_Color_RGB);
	Key_dec Key;
	int Opt=0;
	Key.start();
	while ((Opt=Menu(1,1,&Key))!=4)
	{
		switch (Opt)
		{
		case 0:
			Startgame(&Key);
			break;
		default:
			break;
		}
	}
	
#ifdef _WIN32
	Reset_Win_Required();
#endif
	//重置终端颜色
	Reset_color();
	clean_screen();
	cout << "游戏已结束，按任意键退出游戏";
	cout.flush();
	Key.stop();
	//运行完后恢复光标显示
	dishide_cusor();
	return 0;
}