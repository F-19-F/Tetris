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
int Game_Level = 1;
int First_flag = 0; //用于游戏和信息显示通信
mutex Run;
void Size_detecter(bool *changed, mutex *Run_Lock);
Tetris_UI::Tetris_UI(Size Windows_Size, Size Gsize, Key_dec *key)
{
	thread Decter(Size_detecter, &size_changed, &Run);
	Decter.detach();
	Win_Size = Windows_Size;
	this->Key = key;
	this->Gsize = Gsize;
}
Tetris_UI::~Tetris_UI()
{
	Run.lock();
	//等待大小发现线程退出
	this_thread::sleep_for(std::chrono::milliseconds(60));
}
int Tetris_UI::Dialog(char *option1, char *option2, char *TITLE, char *content)
{
	int num;
	char temp;
	//考虑到兼容性，故采用Buffer缓冲的方式输出内容
	char Buffer[Win_Char_Buffer_Size];
	int buf_point=0;
	int j=0;
	int sum=0;
	clean_screen();
	if ((!option1) && (!option2))
	{
		return -1;
	}
	if (!option2 || !option1)
	{
		num = 1;
	}
	Work_XY(-12, Win_Size.r / 5);
	Title;
	Update_XY(-(int)(strlen(TITLE)/2),Win_Size.r / 5 + 7);
	cursor_move(x,y);
	cout<<TITLE;
	Update_XY(-12,Win_Size.r / 5 + 9);
	cursor_move(x,y);
	for(int i=0;i<(int)(strlen(content));i++)
	{
		if ((temp=content[i])!='\n')
		{
			Buffer[buf_point++] = content[i];
		}
		else
		{
			++j;
			cursor_move(x,y+j);
			Buffer[buf_point]='\0';
			cout << Buffer;
			memset(Buffer, 0, 100 * sizeof(char));
			buf_point = 0;
		}
	}
	cout.flush();
	while (1)
	{
		switch (Key->pop())
		{
		case space:
		case enter:
			return 0;
		break;
		default:
			break;
		}
		this_thread::sleep_for(std::chrono::milliseconds(60));
	}
	return 0;
}
int Tetris_UI::Over()
{
	color(1);
	clean_screen();
	Work_XY(-25, Win_Size.r / 2 - 4);
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
	cout << "                      你的分数:" << Core->get_score();
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
	cout << "                      你的分数:" << Core->get_score();
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
int Tetris_UI::Setting()
{
	hide_cursor();
	clean_screen();
	Work_XY(-12, Win_Size.r / 5);
	Title;
	int Cur_Location_Y = y + 8;
	cursor_move(x + 8, y + 7);
	cout << "游戏难度";
	cursor_move(x + 11, y + 8);
	cout << Game_Level << " ";
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
			if (Game_Level != MAX_LEVEL)
			{
				Game_Level++;
			}
			break;
		case space:
		case enter:
			return 0;
		}
		cursor_move(x + 11, y + 8);
		cout << Game_Level << " ";
		cout.flush();
	}

	return 0;
}
int Tetris_UI::Menu()
{
	hide_cursor();
	color(7);
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
	//cursor_move(x + 16, Cur_Location_Y);
	//cout << "<--";
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
			//cursor_move(x + 16, Cur_Location_Y);
			//cout << "<--";
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
			//cursor_move(x + 16, Cur_Location_Y);
			//cout << "<--";
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
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
int Tetris_UI::Infor(model *next_model)
{
	hide_cursor();
	Work_XY(Gsize.c - 17, 3);
	static model Last_model = *next_model;
	if (First_flag != 0)
	{
		cursor_move(x, y + 7);
		Last_model.print_model(true);
	}
	color(7);
	cursor_move(x, y);
	cout << "Score:" << Core->get_score();
	cout.flush();
	cursor_move(x, y + 3);
	cout << "Level:" << Core->get_speed();
	cout.flush();
	cursor_move(x, y + 6);
	cout << "Next:";
	cout.flush();
	cursor_move(x, y + 7);
	next_model->print_model(false);
	Last_model = *next_model;
	First_flag = 1;
	return 0;
}
int Tetris_UI::Start()
{
	int i;
	int score;
	model *a;
	model *temp;
	Work_XY(-22, 0);
	srand((unsigned)time(NULL));
	if (Is_Cofig_file((char *)OutPutName))
	{
		Core = Restore_Core((char *)OutPutName);
		Core->Editoffset(x,y);
	}
	else
	{
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
		temp = new model(i);
		Infor(temp);
		Core->Add_model(a, Key);
		delete a;
		delete temp;
		Key->clean();
		if (size_changed)
		{
			UpdateSize();
			Update_XY(-22, 0);
			Core->Editoffset(x, y);
			clean_screen();
			size_changed=false;
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