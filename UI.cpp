/*这里是界面类的实现*/
#include <iostream>
#include <thread>
#include <mutex>
#include <cstring>
#ifndef _WIN32
#include "include/ANSI_control.hpp" //linux/unix
#else
#include "include/WinAPI_control.hpp"
#endif
#include "include/UI.hpp"
#include "include/Size.hpp"
using namespace std;
#define TorF(Val_Name) (Val_Name ? "是" : "否")
#define LR_cur(X_off, Y_off, len)            \
	cursor_move(x + X_off, y + Y_off);       \
	cout << "<";                             \
	cursor_move(x + X_off + len, y + Y_off); \
	cout << ">";
#define ChSwtich(Val_Name) \
	if (Val_Name)          \
	{                      \
		Val_Name = false;  \
	}                      \
	else                   \
	{                      \
		Val_Name = true;   \
	}
int Game_Level = 1;
int First_flag = 0; //用于游戏和信息显示通信
mutex Run;
Tetris_UI *_UI;
void Size_detecter(bool *changed, mutex *Run_Lock);
Tetris_UI::Tetris_UI(Size Windows_Size, Size Gsize, Key_det *key)
{
	thread Detecter(Size_detecter, &size_changed, &Run);
	Detecter.detach();
	Win_Size = Windows_Size;
	this->Key = key;
	this->Gsize = Gsize;
	cur_config.Game_ini_Level = 1;
	cur_config.Open_Delay = false;
	cur_config.Open_Increase = true;
	cur_config.Open_Se = true;
	_UI = this;
}
Tetris_UI::~Tetris_UI()
{
	Run.lock();
	//等待大小发现线程退出
	this_thread::sleep_for(std::chrono::milliseconds(60));
	Run.unlock();
}
int Tetris_UI::Setting()
{
	int item = 0;
	hide_cursor();
Sudden_Changed:
	clean_screen();
	Work_XY(-12, Win_Size.r / 5);
	Title;
	int Cur_Location_Y = y + 12;
	cursor_move(x + 8, y + 6);
	cout << "游戏设置";
	cursor_move(x + 6, y + 9);
	cout << "起始下降速度";
	LR_cur(8, 12, 7);
	cursor_move(x + 4, y + 14);
	cout << "是否开启消去动画";
	LR_cur(8, 16, 7);
	cursor_move(x + 11, y + 16);
	cursor_move(x + 1, y + 18);
	cout << "是否随分数自动提升速度";
	LR_cur(8, 20, 7);
	cursor_move(x + 11, y + 20);
	cursor_move(x + 1, y + 22);
	cout << "左右移动时是否延迟下落";
	LR_cur(8, 24, 7);
	cursor_move(x + 11, y + 24);
	while (1)
	{
		Key->MutexLock(true);
		switch (Key->pop())
		{
		case up:
			cursor_move(x + 3, y + 12 + item * 4);
			cout << "   ";
			if (item == 0)
			{
				item = 3;
			}
			else
			{
				item--;
			}
			break;
		case down:
			cursor_move(x + 3, y + 12 + item * 4);
			cout << "   ";
			if (item == 3)
			{
				item = 0;
			}
			else
			{
				item++;
			}
			break;
		case left:
			switch (item)
			{
			case 0:
				if (cur_config.Game_ini_Level != 1)
				{
					cur_config.Game_ini_Level--;
				}
				break;
			case 1:
				ChSwtich(cur_config.Open_Se);
				break;
			case 2:
				ChSwtich(cur_config.Open_Increase);
				break;
			case 3:
				ChSwtich(cur_config.Open_Delay);
				break;
			default:
				break;
			}
			break;
		case right:
			switch (item)
			{
			case 0:
				if (cur_config.Game_ini_Level != MAX_LEVEL)
				{
					cur_config.Game_ini_Level++;
				}
				break;
			case 1:
				ChSwtich(cur_config.Open_Se);
				break;
			case 2:
				ChSwtich(cur_config.Open_Increase);
				break;
			case 3:
				ChSwtich(cur_config.Open_Delay);
				break;
			default:
				break;
			}
			break;
		case space:
		case enter:
			Key->MutexLock(false);
			return 0;
		}
		Key->MutexLock(false);
		cursor_move(x + 11, y + 12);
		cout << cur_config.Game_ini_Level << " ";
		cursor_move(x + 11, y + 16);
		cout << TorF(cur_config.Open_Se);
		cursor_move(x + 11, y + 20);
		cout << TorF(cur_config.Open_Increase);
		cursor_move(x + 11, y + 24);
		cout << TorF(cur_config.Open_Delay);
		cursor_move(x + 3, y + 12 + item * 4);
		cout << "-->";
		cout.flush();
		Act_Change;
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
int Tetris_UI::Infor(bool pause)
{
	hide_cursor();
	Work_XY(Gsize.c - 17, 3);
	cursor_move(x, y + 14);
	if (pause)
	{
		color(1);
		cout << "已暂停!\0";
		cout.flush();
	}
	else
	{
		cout << "           ";
		cout.flush();
	}
	return 0;
}

int Tetris_UI::Infor()
{
	hide_cursor();
	Work_XY(Gsize.c - 17, 3);
	static model Last_model = *NEXT;
	if (First_flag != 0)
	{
		cursor_move(x + 2, y + 9);
		Last_model.print_model(true);
	}
	color(7);
	cursor_move(x, y);
	cout << "分数:   " << Core->get_score() << "  ";
	cout.flush();
	cursor_move(x, y + 3);
	cout << "等级:   " << Core->get_speed() << "  ";
	cout.flush();
	cursor_move(x, y + 6);
	cout << "下一个:";
	cout.flush();
	color(1);
	for (int i = 0; i < 6; i++)
	{
		cursor_move(x + i, y + 7);
		cout << "─" << endl;
		cursor_move(x + i, y + 13);
		cout << "─" << endl;
	}
	cursor_move(x - 1, y + 7);
	cout << "┌";
	cursor_move(x + 6, y + 7);
	cout << "┐";
	cursor_move(x - 1, y + 13);
	cout << "└";
	cursor_move(x + 6, y + 13);
	cout << "┘";
	for (int i = 1; i < 6; i++)
	{
		cursor_move(x - 1, y + 7 + i);
		cout << "│";
		cursor_move(x + 6, y + 7 + i);
		cout << "│";
	}
	cout.flush(); //清空缓冲区
	cursor_move(x + 2, y + 9);
	NEXT->print_model(false);
	Last_model = *NEXT;
	First_flag = 1;
	return 0;
}
int Tetris_UI::Start(bool Reco)
{
	int i;
	int score;
	model *a;
	Work_XY(-22, 0);
	srand((unsigned)time(NULL));
	if (Reco)
	{
		Core = Restore_Core((char *)OutPutName);
		Core->Editoffset(x, y);
	}
	else
	{
		remove(OutPutName);
		Core = new Tetris_Core(Gsize.r, Gsize.c, x, y, cur_config.Game_ini_Level,cur_config);
	}
	clean_screen();
	cout.flush();
	Core->Core_Print();
	i = rand() % 7 + 1;
	while (!Core->over)
	{
		hide_cursor();
		a = new model(i);
		i = rand() % 7 + 1;
		NEXT = new model(i);
		Infor();
		Core->Add_model(a, Key);
		delete a;
		delete NEXT;
		Key->clean();
		if (size_changed)
		{
			UpdateSize();
			Update_XY(-22, 0);
			Core->Editoffset(x, y);
			clean_screen();
			size_changed = false;
			hide_cursor();
			Core->Core_Print();
		}
	}

	First_flag = 0;
	score = Core->get_score();
	Over();
	delete Core;
	return score;
}
int Tetris_UI::UpdateSize()
{
	Win_Size = Getsize();
	return 0;
}