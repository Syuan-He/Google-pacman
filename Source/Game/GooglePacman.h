#pragma once
#include "../Library/gameutil.h"

using namespace game_framework;

//partial
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
class UIObject : public CMovingBitmap{
private:
public:
	UIObject(int x = 25, int y = 100);
	~UIObject() {};

	//位移
	TwoEleContainer window_shift;
};

//多個CMovingBitmap組
class MultUIObj{
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
	int portal_position[2][2] = { { 1, 8 }, { 60, 8 } };
	//地圖陣列
	vector<vector<int>> gameMap;
public:
	GameMap() {};
	~GameMap() {};

	//位移
	TwoEleContainer window_shift{ 25, 100 };
	//地同長度
	TwoEleContainer map_len;
	void map_loader(string str);
	int* portal_detect(int x, int y);

	//回傳地圖陣列值
	const vector<int>& operator[](int i) const;
};

//pacman
class GamePacman: public CMovingBitmap{
private:
	//目前移動方向
	int dir_now = 2;
	//期望移動方向
	int dir_waitfor = 2;
	//pacman位置
	int position[2] = { 37, 15 };
	//移動步數(最大16)
	int total_step = 0;
	//pacman移動速動
	int velocity = 2;

	//參考地圖
	GameMap gameMap;
public:
	GamePacman() {};
	~GamePacman() {};
	
	//血條
	MultUIObj hearts_icon{ 3 , 25, 388};
	//位移
	TwoEleContainer window_shift{ 19, 96 };
	
	void move();
	void update_position(int dir);
	bool CanMove(int dir);

	void set_dir_waitfor(int dir);
	void set_pacman_pos(int* pos);
	void set_game_map(const GameMap& map_t);

	void show_heart_icon(int size);

	//回傳pacman位置
	int& operator[](int index);
};

//鬼
class GameGhost : public CMovingBitmap
{
private:
	GameMap gameMap;
public:
	bool isVaildNode(int x, int y, int xx, int yy);
	int astar(int x0, int y0, int x1, int y1);

	void set_game_map(const GameMap& map_t);
};

//分數
class GameScore
{
private:
	//豆子
	vector<CMovingBitmap*> coins;
	//豆子位置對照表
	map<pair<int, int>, int> coins_map;
	//大力丸
	vector<CMovingBitmap> power_pellets;
	
	//豆子總數
	int total_coin_nums = 0;
	//分數數值
	int score = 0;
	//位移
	int window_shift[2] = { 31, 106 };
public:
	//分數條
	MultUIObj game_scores{ 5, 25, 70 }; 

	void add_coin(CMovingBitmap* c);
	void add_map_point(pair<int, int> p, int v);
	void add_power_pellets(const CMovingBitmap& c);
	void set_coin_nums(int increase);

	int get_coin_nums();

	void show_coins();
	void show_power_pellets();
	void show_score(int size);
	void get_point(GamePacman obj); 
	void get_power(GamePacman obj);
};