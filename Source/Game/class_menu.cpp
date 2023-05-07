#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <memory>

using namespace game_framework;

GameMenu::GameMenu(int menu_wd_x, int menu_wd_y, int target_wd_x, int target_wd_y, int l_space) {
	menu.window_shift.set_value(menu_wd_x, menu_wd_y);
	target.window_shift.set_value(target_wd_x, target_wd_y);
	line_space = l_space;
}

//設定menu游標位置(mode 1: 選項右側)
bool GameMenu::set_target_pos(int increase, int mode) {
	if ((target_pos + increase) >= 0 && (target_pos + increase) < menu.get_nums()) {
		target_pos += increase;
		if (mode == 1) {
			target.SetTopLeft(menu[target_pos].GetLeft() + menu[target_pos].GetWidth(), menu[target_pos].GetTop() + (menu[target_pos].GetHeight() - target.GetHeight()) / 2);
		}
		return true;
	}
	return false;
}

//加載選項
void GameMenu::load_menu(vector<string> source) {
	int i = 0;
	for (string str : source) {
		char *cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());
		unique_ptr<CMovingBitmap> t(new CMovingBitmap);

		t -> LoadBitmapA(cstr, RGB(0, 0, 0));
		t -> SetTopLeft(menu.window_shift[0], menu.window_shift[1] + line_space * i);

		menu.add_obj(*t);
		menu.set_nums(1);

		i ++;
		delete[] cstr;
	}
}

//取得游標位址
int GameMenu::get_target_pos() {
	return target_pos;
}

//顯示menu
void GameMenu::show_menu() {
	for (int i = 0; i < menu.get_nums(); i ++) {
		menu[i].ShowBitmap();
	}
	target.ShowBitmap();
}
