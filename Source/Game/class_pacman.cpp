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

using namespace game_framework;

//移動和位置更新
void GamePacman::move() {
	//if pacman had took a step(one step = 16px)
	if (total_step == 16) {
		//updte pacman's position on the map
		update_position(dir_now);

		//if the position that pacman prefer is executable 
		if (CanMove(dir_waitfor)) {
			//change the diraction to the new one
			dir_now = dir_waitfor;
			//set pacman's picture to the orther one that match with the diraction
		}
		//reset pacman's total step
		total_step = 0;
	}
	//pacman's animetion when it move
	if (total_step % 16 < 5) {
		this->SetFrameIndexOfBitmap(dir_now * 2 + 1);
	}
	else if (total_step % 16 < 10) {
		this->SetFrameIndexOfBitmap(dir_now * 2 + 2);
	}
	else {
		this->SetFrameIndexOfBitmap(0);
	}
	//if the diraction now is executable keep going
	if (CanMove(dir_now)) {
		switch (dir_now)
		{
		case 0:
			this->SetTopLeft(this->GetLeft() + velocity, this->GetTop());
			break;
		case 1:
			this->SetTopLeft(this->GetLeft(), this->GetTop() - velocity);
			break;
		case 2:
			this->SetTopLeft(this->GetLeft() - velocity, this->GetTop());
			break;
		case 3:
			this->SetTopLeft(this->GetLeft(), this->GetTop() + velocity);
			break;
		default:
			break;
		}
		total_step += velocity;
	}
	//if pacman hit the wall (include portal)
	else {
		////check that if pacman hit a portal
		int* t = gameMap.portal_detect(position[0], position[1]);
		if (t != nullptr) {
			position[0] = t[0];
			position[1] = t[1];
			this->SetTopLeft(16 * (position[0] - 2) + window_shift[0], 16 * position[1] + window_shift[1]);
		}
		////check that if the position that pacman prefer is executable 
		//else 
		if (CanMove(dir_waitfor)) {
			//change the diraction
			dir_now = dir_waitfor;
		}
	}
}

//更新位置
void GamePacman::update_position(int dir) {
	switch (dir)
	{
	case 0:
		position[0] ++;
		break;
	case 1:
		position[1] --;
		break;
	case 2:
		position[0] --;
		break;
	case 3:
		position[1] ++;
		break;
	default:
		// warning wrong dir
		break;
	}
}

//方向是否可移動
bool GamePacman::CanMove(int dir) {
	int x = position[0], y = position[1];

	switch (dir)
	{
	case 0:
		if (gameMap[y][x + 1] == 1) {
			return false;
		}
		else {
			return true;
		}
	case 1:
		if (gameMap[y - 1][x] == 1) {
			return false;
		}
		else {
			return true;
		}
	case 2:
		if (gameMap[y][x - 1] == 1) {
			return false;
		}
		else {
			return true;
		}
	case 3:
		if (gameMap[y + 1][x] == 1) {
			return false;
		}
		else {
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

//dir_waitfor設定
void GamePacman::set_dir_waitfor(int dir) {
	dir_waitfor = dir;
}

//position設定
void GamePacman::set_pacman_pos(int* pos) {
	position[0] = pos[0];
	position[1] = pos[1];
	this->SetTopLeft(16 * (pos[0] - 2) - 6 + window_shift[0], 16 * pos[1] - 6 + window_shift[1]);
}

//參考地圖設定(在設定完後才能使用gameMap)
void GamePacman::set_game_map(const GameMap& map_t) {
	gameMap = map_t;
}

//血條顯示
void GamePacman::show_heart_icon(int size) {
	for (int i = 0; i < hearts_icon.get_nums(); i++) {
		hearts_icon[i].ShowBitmap(size);
	}
}

//回傳pacman位置
int& GamePacman::operator[](int index) {
	return position[index];
}