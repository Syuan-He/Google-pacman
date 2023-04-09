#include "stdafx.h"
#include "character.h"

using namespace game_framework;

//Character::~Character() {}

void Character::move() {
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
		int* t = gameMap.portal_detect(position[0], position[1]);
		if (t != nullptr) {
			position[0] = t[0];
			position[1] = t[1];
			this->SetTopLeft(16 * (position[0] - 2) + window_shift[0], 16 * position[1] + window_shift[1]);
		}
		//check that if the position that pacman prefer is executable 
		else if (CanMove(dir_waitfor)) {
			//change the diraction
			dir_now = dir_waitfor;
		}
	}
}

bool Character::CanMove(int dir) {
	switch (dir)
	{
	case 0:
		if (gameMap[position[1]][position[0] + 1] == 1) {
			return false;
		}
		else {
			return true;
		}
	case 1:
		if (gameMap[position[1] - 1][position[0]] == 1) {
			return false;
		}
		else {
			return true;
		}
	case 2:
		if (gameMap[position[1]][position[0] - 1] == 1) {
			return false;
		}
		else {
			return true;
		}
	case 3:
		if (gameMap[position[1] + 1][position[0]] == 1) {
			return false;
		}
		else {
			return true;
		}
	default:
		break;
	}
	return false;
}

//if object change the direction , will it hit the wall
//return: true(it won't), false(it will)
//update object's position by the input direction 
void Character::update_position(int dir) {
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

//dir_waitfor設定
void Character::set_dir_waitfor(int dir) {
	dir_waitfor = dir;
}

void Character::setPos(int x, int y) {
	position[0] = x;
	position[1] = y;
	this->SetTopLeft(16 * (x - 2) + window_shift[0], 16 * y + window_shift[1]);
}

//參考地圖設定(在設定完後才能使用gameMap)
void Character::set_game_map(const GameMap& map_t) {
	gameMap = map_t;
}

void Character::set_inital(int x, int y, int index) {
	initial_pos[0] = x;
	initial_pos[1] = y;
	initial_frame_index = index;
}

void Character::initialize() {
	this->setPos(initial_pos[0], initial_pos[1]);
	this->SetFrameIndexOfBitmap(initial_frame_index);
	total_step = 0;
	dir_now = 2;				//目前移動方向
	dir_waitfor = 2;
}

//回傳pacman位置
int& Character::operator[](int index) {
	return position[index];
}