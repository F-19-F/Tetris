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
int Game_Level=1;
Size _Gsize;
int Over(int x,int y,int Score,Key_dec *Key)
{
	color(1);
	clean_screen();
	cursor_move(x,y);
	#ifdef _WIN32
	cout<<"  ____                         ___";
	movedown(1);
	cout<<" / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __";
	movedown(1);
	cout<<"| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|";
	movedown(1);
	cout<<"| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |";
	movedown(1);
	cout<<" \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|";
	movedown(1);
	cout<<"                      你的分数:"<<Score;
	movedown(5);
	cout<<"                 按空格键退出当前界面";
	#else
	cout<<"  ____                         ___";
	cursor_move(x,y);
	movedown(1);
	cout<<" / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __";
	cursor_move(x,y);
	movedown(2);
	cout<<"| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|";
	cursor_move(x,y);
	movedown(3);
	cout<<"| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |";
	cursor_move(x,y);
	movedown(4);
	cout<<" \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|";
	cursor_move(x,y);
	movedown(5);
	cout<<"                      你的分数:"<<Score;
	cursor_move(x,y);
	movedown(10);
	cout<<"                 按空格键退出当前界面";
	#endif
	while (Key->pop()!=space)
	{
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}
int Setting(int x, int y, Key_dec *Key)
{
	hide_cursor();
	clean_screen();
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
	cout << " _____    _        _";
	cursor_move(x, y);
	movedown(1);
	cout << "|_   _|__| |_ _ __(_)___";
	cursor_move(x, y);
	movedown(2);
	cout << "  | |/ _ \\ __| '__| / __|";
	cursor_move(x, y);
	movedown(3);
	cout << "  | |  __/ |_| |  | \\__ \\";
	cursor_move(x, y);
	movedown(4);
	cout << "  |_|\\___|\\__|_|  |_|___/";
	cursor_move(x, y);
	movedown(5);
#endif
	return 0;
}
int Menu(int x, int y, Key_dec *Key)
{
	hide_cursor();
	color(7);
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
	cout << " _____    _        _";
	cursor_move(x, y);
	movedown(1);
	cout << "|_   _|__| |_ _ __(_)___";
	cursor_move(x, y);
	movedown(2);
	cout << "  | |/ _ \\ __| '__| / __|";
	cursor_move(x, y);
	movedown(3);
	cout << "  | |  __/ |_| |  | \\__ \\";
	cursor_move(x, y);
	movedown(4);
	cout << "  |_|\\___|\\__|_|  |_|___/";
	cursor_move(x, y);
	movedown(5);
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
			break;
		default:
			break;
		}
		cout.flush();
		Key->MutexLock(false);
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void Infor_print(int x, int y, game_core *core, model *next_model)
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
int Startgame(int x , int y,Size Gsize,Key_dec *Key)
{
	int i;
	model *a;
	model *temp;
	Size ini_size;
	ini_size = Getsize();
	srand((unsigned)time(NULL));
	game_core b(Gsize.r, Gsize.c, x, y, 10);
	clean_screen();
	cout.flush();
	b.print();
	i = rand() % 7 + 1;
	while (!b.over)
	{
		hide_cursor();
		a = new model(i);
		i = rand() % 7 + 1;
		temp = new model(i);
		Infor_print(x + Gsize.c + 5,y + 3, &b, temp);
		b.Add_model(a, Key);
		delete a;
		delete temp;
		Key->clean();
		b.print();
	}
	cursor_move(ini_size.r, ini_size.c);
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
	Size Win_size=Getsize();//获取窗口大小
	_Gsize.r=27;
	_Gsize.c=18;
	Key_dec Key;
	int Opt=0;
	Key.start();
	while ((Opt=Menu(Win_size.c/2-12,Win_size.r/5,&Key))!=4)//Logo居中
	{
		switch (Opt)
		{
		case 0:
			Over (Win_size.c/2-25,Win_size.r/2-4,Startgame(Win_size.c/2-22,0,_Gsize,&Key),&Key);
			break;
		case 1:
			Setting(Win_size.c/2-12,Win_size.r/5,&Key);
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