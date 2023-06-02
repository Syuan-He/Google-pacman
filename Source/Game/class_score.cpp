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

//加入豆子
void GameScore::add_coin(CMovingBitmap c) {
	coins.push_back(c);
}

//加入大力丸
void  GameScore::add_power_pellets(const CMovingBitmap& c) {
	power_pellets.push_back(c);
}

//設定豆子數量(mode 0: 採累加, mode 採直接設定)
void GameScore::set_coin_nums(int increase, int mode) {
	if (mode == 0) {
		total_coin_nums += increase;
	}
	else if (mode == 1) {
		total_coin_nums = increase;
	}
}

void GameScore::set_window_shift(int x, int y) {
	window_shift[0] = x;
	window_shift[1] = y;
}

//取得豆子數量
int GameScore::get_coin_nums() {
	return total_coin_nums;
}

int GameScore::get_score() {
	return score;
}

//顯示豆子
void GameScore::show_coins() {
	for (CMovingBitmap t : coins) {
		t.ShowBitmap();
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
bool GameScore::get_point(GamePacman obj) {
	for (auto it = coins.begin(); it != coins.end(); it ++) {
		if (obj.IsOverlap(obj, *it)) {
			it = coins.erase(it);
			total_coin_nums--;
			score += 10;

			return true;
		}
	}
	return false;
}

bool GameScore::get_power(GamePacman obj) {
	for (auto it = power_pellets.begin(); it != power_pellets.end(); it ++) {
		if (obj.IsOverlap(obj, *it)) {
			it = power_pellets.erase(it);
			score += 10;
			return true;
		}
	}
	return false;
}

void GameScore::get_ghost(GamePacman obj, GameGhost ghost, int catchTime) {
	if (obj.IsOverlap(obj, ghost)) {
		ghost.isChoas = 2;
		ghost.choasFlash = false;
		int plusPoint = 200;
		for (int i = 0; i < catchTime; i++) {
			plusPoint *= 2;
		}
		score += plusPoint;
	}
}

void GameScore::initialize(GameMap Map) {
	coins.clear();
	power_pellets.clear();
	set_coin_nums(0, 1);

	//豆子,大力丸初始化
	for (int i = 0; i < Map.map_len[0]; i++) {
		for (int j = 0; j < Map.map_len[1]; j++) {
			//為0的道路加入豆子
			if (Map[i][j] == 0) {
				unique_ptr<CMovingBitmap> t(new CMovingBitmap);
				t->LoadBitmapA("Resources/words/coin.bmp");
				t->SetTopLeft(16 * (j - 2) + 6 + window_shift[0], 16 * i + 6 + window_shift[1]);
				add_coin(*t);
				set_coin_nums(1);
			}
			//為3的道路加入大力丸
			else if (Map[i][j] == 3) {
				unique_ptr<CMovingBitmap> t(new CMovingBitmap);
				t->LoadBitmapA("Resources/words/dot.bmp");
				t->SetTopLeft(16 * (j - 2) + 4 + window_shift[0], 16 * i + 4 + window_shift[1]);
				add_power_pellets(*t);
			}
		}
	}
	score = 0;
}