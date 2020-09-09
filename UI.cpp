#include <iostream>
#include <thread>
#ifndef _WIN32
#include "include/ANSI_control.hpp" //linux/unix
#else
#include "include/WinAPI_control.hpp"
#endif
#include "include/Tetris_Core.hpp"
#include "include/UI.hpp"
#include "include/Size.hpp"
using namespace std;
int Game_Level = 1;
int First_flag = 0; //用于游戏和信息显示通信
int Over(int x, int y, int Score, Key_dec *Key)
{
	color(1);
	clean_screen();
	cursor_move(x, y);
#ifdef _WIN32
	cout << "  ____                         ___";
	movedown(1);
	cout << " / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __";
	movedown(1);
	cout << "| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|";
	movedown(1);
	cout << "| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |";
	movedown(1);
	cout << " \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|";
	movedown(1);
	cout << "                      你的分数:" << Score;
	movedown(5);
	cout << "                 按空格键退出当前界面";
#else
	cout << "  ____                         ___";
	cursor_move(x, y);
	movedown(1);
	cout << " / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __";
	cursor_move(x, y);
	movedown(2);
	cout << "| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|";
	cursor_move(x, y);
	movedown(3);
	cout << "| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |";
	cursor_move(x, y);
	movedown(4);
	cout << " \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|";
	cursor_move(x, y);
	movedown(5);
	cout << "                      你的分数:" << Score;
	cursor_move(x, y);
	movedown(10);
	cout << "                 按空格键退出当前界面";
#endif
	cout.flush();
	while (Key->pop() != space)
	{
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}
int Setting(int x, int y, Key_dec *Key)
{
	hide_cursor();
	clean_screen();
	Title;
	int Cur_Location_Y = y + 8;
	cursor_move(x + 8, y + 7);
	cout << "游戏难度";
	cursor_move(x + 11, y + 8);
	cout << Game_Level<<" ";
	cursor_move(x + 8, y + 8);
	cout << "<";
	cursor_move(x + 15, y + 8);
	cout << ">";
	while (1)
	{
		switch (Key->pop())
		{
		case left:
			if (Game_Level != 1)
			{
				Game_Level--;
			}
			break;
		case right:
			if (Game_Level !=MAX_LEVEL)
			{
				Game_Level++;
			}
			break;
		case space:
			return 0;
		}
		cursor_move(x + 11, y + 8);
		cout << Game_Level<<" ";
		cout.flush();
	}

	return 0;
}
int Menu(int x, int y, Key_dec *Key)
{
	hide_cursor();
	color(7);
	clean_screen();
	int Cur_Location_Y = y + 7;
	Title;
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
			break;
		default:
			break;
		}
		cout.flush();
		Key->MutexLock(false);
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void Infor_print(int x, int y, Tetris_Core *core, model *next_model)
{
	hide_cursor();
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
int Startgame(int x, int y, Size Gsize, Key_dec *Key)
{
	int i;
	int score;
	model *a;
	model *temp;
	Size ini_size;
	ini_size = Getsize();
	Tetris_Core *core;
	srand((unsigned)time(NULL));
	if (Is_Cofig_file((char *)OutPutName))
	{
		core=new Tetris_Core((char*)OutPutName);
	}
	else
	{
		core=new Tetris_Core(Gsize.r, Gsize.c, x, y, Game_Level,true);
	}
	clean_screen();
	cout.flush();
	core->Core_Print();
	i = rand() % 7 + 1;
	while (!core->over)
	{
		hide_cursor();
		a = new model(i);
		i = rand() % 7 + 1;
		temp = new model(i);
		Infor_print(x + Gsize.c + 5, y + 3, core, temp);
		core->Add_model(a, Key);
		delete a;
		delete temp;
		Key->clean();
	}
	cursor_move(ini_size.r, ini_size.c);
	First_flag = 0;
	score=core->get_score();
	delete core;
	return score;
}