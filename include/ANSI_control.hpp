#ifndef _ANSI_C
#define _ANSI_C
#define end_all() cout << "\033[0m"
//高亮度
#define high_light() cout << "\033[1m"
//下划线
#define underline() cout << "\033[4m"
//闪烁
#define blink() cout << "\033[5m"
//反显
#define reserve() cout << "\033[7m"
//消隐
#define blank() cout << "\033[8m"
//清屏
#define clean_screen() cout << "\033[2J"
//清除从光标到行尾的内容
#define clean_line() cout << "\033[K"
//保存光标位置
#define save_cursor() cout << "\033[s"
//恢复光标位置
#define restore_cursor() cout << "\033[u"
//隐藏光标
#define hide_cursor() cout << "\033[?25l"
//显示光标
#define dishide_cusor() cout << "\033[?25h"
// 上移光标
#define moveup(x) cout << "\033[" << (x) << "A"
// 下移光标
#define movedown(x) cout << "\033[" << (x) << "B";
// 左移光标
#define moveleft(y) cout << "\033[" << (y) << "D"
// 右移光标
#define moveright(y) cout << "\033[" << (y) << "C"
// 定位光标
#define cursor_move(x, y) cout << "\033[" << (y) << ";" << (x) << "H"
void Set_Default_color(int r, int g, int b, int f_r, int f_g, int f_b);
void color(int i);
void Reset_color();
#endif
//It seems that ANSI works not as well as winapi.