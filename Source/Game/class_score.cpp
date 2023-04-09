﻿#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

//加入豆子
void GameScore::add_coin(CMovingBitmap* c) {
	coins.push_back(c);
}

//更新豆子位置對照表
void GameScore::add_map_point(pair<int, int> p, int v) {
	coins_map[p] = v;
}

//加入大力丸
void  GameScore::add_power_pellets(const CMovingBitmap& c) {
	power_pellets.push_back(c);
}

//設定豆子數量
void GameScore::set_coin_nums(int increase) {
	total_coin_nums += increase;
}

//取得豆子數量
int GameScore::get_coin_nums() {
	return total_coin_nums;
}

//顯示豆子
void GameScore::show_coins() {
	for (CMovingBitmap* t : coins) {
		if (t != nullptr) {
			t-> ShowBitmap();
		}
	}
}

//顯示大力丸
void GameScore::show_power_pellets() {
	for (CMovingBitmap t : power_pellets) {
		t.ShowBitmap();
	}
}

//顯示分數
void GameScore::show_score(int size) {
	vector<int> s(game_scores.get_nums(), -1);
	int time = 0;

	int t = score;

	while (t > 0) {
		s[time] = t % 10;
		t /= 10;
		time++;
	}
	for (int i : s) {
		if (i != -1) {
			game_scores[time - 1].SetFrameIndexOfBitmap(i);
			game_scores[time - 1].ShowBitmap(size);
		}
		else {
			break;
		}
		time--;
	}
}

//偵測是否吃到豆子
void GameScore::get_point(GamePacman obj) {
	auto t = coins_map.find(pair<int, int>(obj[0], obj[1]));
	if (t != coins_map.end()) {
		auto p = coins[t->second];
		if (p != nullptr) {
			delete p;
			coins[t->second] = nullptr;
			total_coin_nums--;
			score += 10;
		}
	}
}

void GameScore::get_power(GamePacman obj) {
	for (auto it = power_pellets.begin(); it != power_pellets.end();) {
		if (obj.IsOverlap(obj, *it)) {
			it = power_pellets.erase(it);
		}
		else {
			it++;
		}
	}
}