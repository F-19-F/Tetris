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
#include "include/Tetris_Core.hpp"
#include "include/UI.hpp"
#include "include/Size.hpp"
using namespace std;
#define Work_XY(offset_x, offset_y)    \
	int x = Win_Size.c / 2 + offset_x; \
	int y = offset_y;
#define Update_XY(offset_x, offset_y) \
	x = Win_Size.c / 2 + offset_x;    \
	y = offset_y;
#define Act_Change            \
	if (size_changed)         \
	{                         \
		hide_cursor();        \
		UpdateSize();         \
		size_changed = false; \
		goto Sudden_Changed;  \
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
	_UI = this;
}
Tetris_UI::~Tetris_UI()
{
	Run.lock();
	//等待大小发现线程退出
	this_thread::sleep_for(std::chrono::milliseconds(60));
	Run.unlock();
}
int Tetris_UI::Dialog(char *option1, char *option2, char *TITLE, char *content)
{
	int num = 2;
	int choose = 1;
	//考虑到兼容性，故采用Buffer缓冲的方式输出内容
	char Buffer[Win_Char_Buffer_Size];
	int buf_point = 0;
	int j = 0; //用来记录行数
	int sum = 0;
Sudden_Changed:
	clean_screen();
	if ((!option1) && (!option2))
	{
		return -1;
	}
	if (!option1)
	{
		num = -1;
		choose = 2;
	}
	if (!option2)
	{
		num = 1;
		choose = 1;
	}
	Work_XY(-12, Win_Size.r / 5);
	Title;
	Update_XY(-(int)(strlen(TITLE) / 2), Win_Size.r / 5 + 7);
	cursor_move(x, y);
	cout << TITLE;
	Update_XY(-12, Win_Size.r / 5 + 9);
	cursor_move(x, y);
	for (int i = 0; i < (int)(strlen(content)); i++)
	{
		if (content[i] != '\n')
		{
			Buffer[buf_point++] = content[i];
		}
		else
		{
			++j;
			cursor_move(x, y + j);
			Buffer[buf_point] = '\0';
			cout << Buffer;
			memset(Buffer, 0, 100 * sizeof(char));
			buf_point = 0;
		}
	}
	cout.flush();
	if (num != 2)
	{
		x += 10;
		if (num == 1)
		{
			x -= strlen(option1) / 2;
			cursor_move(x - 4, y + j + 4);
			cout << "-->";
			cursor_move(x, y + j + 4);
			cout << option1;
			cout.flush();
		}
		else
		{
			x -= strlen(option2) / 2;
			cursor_move(x - 4, y + j + 4);
			cout << "-->";
			cursor_move(x, y + j + 4);
			cout << option2;
			cout.flush();
		}
	}
	else
	{
		cursor_move(x, y + j + 4);
		cout << "-->";
		cursor_move(x + 4, y + j + 4);
		cout << option1;
		cursor_move(x + 24, y + j + 4);
		cout << option2;
		cout.flush();
	}
	while (1)
	{
		Key->MutexLock(true);
		switch (Key->pop())
		{
		case left:
			if (num == 2)
			{
				if (choose == 2)
				{
					choose = 1;
					cursor_move(x + 20, y + j + 4);
					cout << "   ";
					cursor_move(x, y + j + 4);
					cout << "-->";
					cout.flush();
				}
			}
			break;
		case right:
			if (num == 2)
			{
				if (choose == 1)
				{
					choose = 2;
					cursor_move(x, y + j + 4);
					cout << "   ";
					cursor_move(x + 20, y + j + 4);
					cout << "-->";
					cout.flush();
				}
			}
			break;
		case space:
		case enter:
			Key->MutexLock(false);
			return choose;
			break;
		default:
			break;
		}
		Key->MutexLock(false);
		Act_Change;
		this_thread::sleep_for(std::chrono::milliseconds(60));
	}
	return 0;
}
int Tetris_UI::Over()
{
	color(1);
Sudden_Changed:
	clean_screen();
	Work_XY(-25, Win_Size.r / 2 - 4);
	cursor_move(x, y);
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
	cout << "                      你的分数:" << Core->get_score();
	cursor_move(x, y);
	movedown(10);
	cout << "                 按空格键退出当前界面";
	cout.flush();
	while (Key->pop() != space)
	{
		Act_Change;
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}
int Tetris_UI::Setting()
{
	hide_cursor();
Sudden_Changed:
	clean_screen();
	Work_XY(-12, Win_Size.r / 5);
	Title;
	int Cur_Location_Y = y + 12;
	cursor_move(x + 8, y + 9);
	cout << "游戏难度";
	cursor_move(x + 11, y + 12);
	cout << Game_Level << " ";
	cursor_move(x + 8, y + 12);
	cout << "<";
	cursor_move(x + 15, y + 12);
	cout << ">";
	while (1)
	{
		Key->MutexLock(true);
		switch (Key->pop())
		{
		case left:
			if (Game_Level != 1)
			{
				Game_Level--;
			}
			break;
		case right:
			if (Game_Level != MAX_LEVEL)
			{
				Game_Level++;
			}
			break;
		case space:
		case enter:
			Key->MutexLock(false);
			return 0;
		}
		Key->MutexLock(false);
		cursor_move(x + 11, y + 12);
		cout << Game_Level << " ";
		cout.flush();
		Act_Change;
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
int Tetris_UI::Menu()
{
	hide_cursor();
	color(7);
Sudden_Changed:
	if (Is_Cofig_file((char *)OutPutName))
	{
		if (Dialog((char *)"是", (char *)"否", (char *)"警告", (char *)Warning) == 1)
		{
			Start(true);
		}
		else
		{
			remove(OutPutName);
		}
	}
	clean_screen();
	Work_XY(-12, Win_Size.r / 5);
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
	while (1)
	{
		Key->MutexLock(true);
		switch (Key->pop())
		{
		case up:
			cursor_move(x + 5, Cur_Location_Y);
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
			break;
		case down:
			cursor_move(x + 5, Cur_Location_Y);
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
			break;
		case space:
		case enter:
			Key->MutexLock(false);
			return ((Cur_Location_Y - y - 7) / 2);
		case esc:
			Key->MutexLock(false);
			return 4;
			break;
		default:
			break;
		}
		cout.flush();
		Key->MutexLock(false);
		Act_Change;
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
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
		Core = new Tetris_Core(Gsize.r, Gsize.c, x, y, Game_Level, true);
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