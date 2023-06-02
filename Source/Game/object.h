#pragma once

#include "../Library/gameutil.h"
#include <array>

using namespace game_framework;

//大小為二的陣列容器
class TwoEleContainer {
private:
	int container[2];
public:
	TwoEleContainer(int x = 0, int y = 0);

	void set_value(int x, int y);
	// operator[]
	int& operator[](int index);
};

//為CMovingBitmap新增功能
class UIObject : public CMovingBitmap {
private:
public:
	UIObject(int x = 25, int y = 100);

	//位移
	TwoEleContainer window_shift;
};

//多個CMovingBitmap組
class MultUIObj {
private:
	int obj_num;
	vector<CMovingBitmap> objs;
public:
	MultUIObj(int num, int x = 25, int y = 100);

	//位移
	TwoEleContainer window_shift;
	void add_obj(const CMovingBitmap& o);
	int get_nums();

	void set_nums(int increase, int mode = 0);
	void clear_objs();
	//回傳第index個CMovingBitmap
	CMovingBitmap& operator[](int index);
};

//地圖
class GameMap {
private:
	//傳送門位置
	vector<array<int, 4>> portal_position;
	//地圖陣列
	vector<vector<int>> gameMap;
	//傳送門(圖像)陣列
	vector<CMovingBitmap> portal_icon;
public:
	//地同長度
	TwoEleContainer map_len;
	//背景
	UIObject Background;

	void map_loader(string str);
	pair<int, int> portal_detect(int x, int y);
	void add_portal_icon(int x, int y);

	void show_portal();
	//回傳地圖陣列值
	const vector<int>& operator[](int i) const;
};

//Menu
class GameMenu {
private:
	int line_space = 60;
	int target_pos = 0;
public:
	GameMenu(int menu_wd_x = 0, int menu_wd_y = 0, int target_wd_x = 0, int target_wd_y = 0, int l_space = 0);
	
	MultUIObj menu{ 0 }; //按鍵
	UIObject target; //指標

	bool set_target_pos(int increase, int mode = 1);
	void load_menu(vector<string> source);

	int get_target_pos();

	void show_menu();
};