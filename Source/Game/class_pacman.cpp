#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

using namespace game_framework;

//pacman的移動
void GamePacman::move() {
	//如果走為一步
	if (total_step == velocity) {
		//更新位置
		update_position(dir_now);

		//更新速度
		if (waitVelocity != velocity) {
			velocity = waitVelocity;
		}

		//更新方向
		if (CanMove(dir_waitfor)) {
			dir_now = dir_waitfor;
		}
		//重置步數
		total_step = 0;
		
		//檢查是否通過傳送門
		pair<int, int> t = gameMap.portal_detect(position[0], position[1]);
		if (t.first != -1) {
			position[0] = t.first;
			position[1] = t.second;
			this->SetTopLeft(16 * (position[0] - 2) + window_shift[0], 16 * position[1] + window_shift[1]);
		}
	}

	//pacman移動動畫
	if (total_step < velocity / 3) {
		this->SetFrameIndexOfBitmap(dir_now * 2 + 1);
	}
	else if (total_step < velocity * 2 / 3) {
		this->SetFrameIndexOfBitmap(dir_now * 2 + 2);
	}
	else {
		this->SetFrameIndexOfBitmap(0);
	}

	//移動
	if (CanMove(dir_now)) {
		switch (dir_now)
		{
		case 0:
			this->SetTopLeft(this->GetLeft() + moving_schedule[total_step], this->GetTop());
			break;
		case 1:
			this->SetTopLeft(this->GetLeft(), this->GetTop() - moving_schedule[total_step]);
			break;
		case 2:
			this->SetTopLeft(this->GetLeft() - moving_schedule[total_step], this->GetTop());
			break;
		case 3:
			this->SetTopLeft(this->GetLeft(), this->GetTop() + moving_schedule[total_step]);
			break;
		default:
			break;
		}
		total_step ++;
	}
	else if (CanMove(dir_waitfor)) {
		dir_now = dir_waitfor;
	}
}

//血條顯示
void GamePacman::show_heart_icon(int size) {
	for (int i = 0; i < hearts_icon.get_nums(); i++) {
		hearts_icon[i].ShowBitmap(size);
	}
}

//取得當前方向
int GamePacman::getDirNow() {
	return dir_now;
}