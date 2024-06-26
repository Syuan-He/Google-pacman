﻿#pragma once

#include "../Library/gameutil.h"
#include <map>

using namespace game_framework;
//分數
class GameScore {
private:
	vector<CMovingBitmap> coins;			//豆子
	vector<CMovingBitmap> power_pellets;	//大力丸
	vector<pair<int, int>> coin_position;

	int total_coin_nums = 0;				//豆子總數
	int score = 0;							//分數數值
	int window_shift[2] = { 31, 106 };		//位移

	GameMap gameMap;
public:
	//分數條
	MultUIObj game_scores{ 5, 25, 70 };

	void add_coin(CMovingBitmap c);
	void add_power_pellets(const CMovingBitmap& c);
	void set_coin_nums(int increase, int mode = 0);
	void set_window_shift(int x, int y);
	void set_game_map(const GameMap& map_t);

	int get_coin_nums();
	int get_score();

	void show_coins();
	void show_power_pellets();
	void show_score(int size);
	bool get_point(GamePacman obj);
	bool get_power(GamePacman obj);
	void get_ghost(GamePacman obj, GameGhost ghost, int catchTime);

	int get_coin_dir(int x, int y);
	int get_power_dir(int x, int y);

	void initialize(GameMap Map);
};
