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
		//check that if pacman hit a portal
		pair<int, int> t = gameMap.portal_detect(position[0], position[1]);
		if (t.first != -1) {
			position[0] = t.first;
			position[1] = t.second;
			this->SetTopLeft(16 * (position[0] - 2) + window_shift[0], 16 * position[1] + window_shift[1]);
		}
		//check that if the position that pacman prefer is executable 
		else if (CanMove(dir_waitfor)) {
			//change the diraction
			dir_now = dir_waitfor;
		}
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