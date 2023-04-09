#pragma once

#include "../Library/gameutil.h"

using namespace game_framework;

//大小為二的陣列容器
class TwoEleContainer {
private:
	int container[2];
public:
	TwoEleContainer(int x = 0, int y = 0);
	~TwoEleContainer() {};

	void set_value(int x, int y);
	// operator[]
	int& operator[](int index);
};

//為CMovingBitmap新增功能
class UIObject : public CMovingBitmap {
private:
public:
	UIObject(int x = 25, int y = 100);
	~UIObject() {};

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
	~MultUIObj() {};

	//位移
	TwoEleContainer window_shift;
	void add_obj(const CMovingBitmap& o);
	int get_nums();

	void set_nums(int increase);

	//回傳第index個CMovingBitmap
	CMovingBitmap& operator[](int index);
};

//地圖
class GameMap {
private:
	//傳送門位置
	int portal_position[2][2];
	//地圖陣列
	vector<vector<int>> gameMap;
public:
	GameMap() {};
	~GameMap() {};

	//地同長度
	TwoEleContainer map_len;
	//背景
	UIObject Background;
	void map_loader(string str);
	int* portal_detect(int x, int y);

	//回傳地圖陣列值
	const vector<int>& operator[](int i) const;
};
