#include "stdafx.h"
#include "character.h"

using namespace game_framework;

//Character::~Character() {};
//角色是否可以往dir移動
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

void Character::update_moving_schedule() {
	for (int i = 0; i < velocity; i++) {
		moving_schedule[i] = int(round(float(16 * (i + 1)) / velocity)) - int(round(float(16 * i) / velocity));
	}
}

//dir_waitfor設定
void Character::set_dir_waitfor(int dir) {
	dir_waitfor = dir;
}

//設定角色在視窗中的位置
void Character::setPos(int x, int y) {
	position[0] = x;
	position[1] = y;
	this->SetTopLeft(16 * (x - 2) + window_shift[0], 16 * y + window_shift[1]);
}

//設定速度
void  Character::setVelocity(int v) {
	waitVelocity = v;
}

//參考地圖設定(在設定完後才能使用gameMap)
void Character::set_game_map(const GameMap& map_t) {
	gameMap = map_t;
}

//設定初始化數值
void Character::set_inital(int x, int y, int w_x, int w_y, int index) {
	initial_pos[0] = x;
	initial_pos[1] = y;
	window_shift.set_value(w_x, w_y);
	initial_frame_index = index;
}

//初始化
void Character::initialize() {
	this->setPos(initial_pos[0], initial_pos[1]);
	this->SetFrameIndexOfBitmap(initial_frame_index);
	total_step = 0;
	dir_now = 0;				//目前移動方向
	dir_waitfor = 0;
	velocity = 2;
	waitVelocity = 2;
	update_moving_schedule();
}

//回傳速度
int Character::get_velocity() {
	return velocity;
}

//回傳pacman位置
int& Character::operator[](int index) {
	return position[index];
}

