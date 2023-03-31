#include "stdafx.h"
#include "character.h"

using namespace game_framework;

void character::setPos(int x, int y){
	position[0] = x;
	position[1] = y;
}
//character::~character() {}

int character::getX() {
	return position[0];
}

int character::getY() {
	return position[1];
}

void character::move(int** gameMap, int portal_pos[2][2], int win_shift[2]) {
	//if pacman had took a step(one step = 16px)
	if (total_step == 16) {
		//updte pacman's position on the map
		update_position(dir_now);

		//if the position that pacman prefer is executable 
		if (CanMove(dir_waitfor, gameMap)) {
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
	if (CanMove(dir_now, gameMap)) {
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
		if (portal_detect(portal_pos, win_shift)) {}
		//check that if the position that pacman prefer is executable 
		else if (CanMove(dir_waitfor, gameMap)) {
			//change the diraction
			dir_now = dir_waitfor;
		}
	}
}

bool character::CanMove(int dir, int** gameMap) {
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
		break;
	default:
		break;
	}
	return false;
}

//if object change the direction , will it hit the wall
//return: true(it won't), false(it will)
//update object's position by the input direction 
void character::update_position(int dir) {
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

//portal dectect and sending object to the other side
//return: true(object hit the portal), false(object didn't hit the portal)
bool character::portal_detect(int portal_pos[2][2], int window_shift[2]) {
	if (this->position[0] == portal_pos[0][0] && this->position[1] == portal_pos[0][1]) {
		this->position[0] = portal_pos[1][0];
		this->position[1] = portal_pos[1][1];
		SetTopLeft(16 * (this->position[0] - 2) - 6 + window_shift[0], 16 * this->position[1] - 6 + window_shift[1]);
		return true;
	}
	else if (this->position[0] == portal_pos[1][0] && this->position[1] == portal_pos[1][1]) {
		this->position[0] = portal_pos[0][0];
		this->position[1] = portal_pos[0][1];
		SetTopLeft(16 * (this->position[0] - 2) - 6 + window_shift[0], 16 * this->position[1] - 6 + window_shift[1]);
		return true;
	}

	return false;
}