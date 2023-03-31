#pragma once

#include "../Library/gameutil.h"

using namespace game_framework;

class character : public game_framework::CMovingBitmap {
private:
	int dir_now = 2;							//pacman's dir
	int position[2];							//pacman's pos
	int total_step = 0;							//pacman's total step
	int velocity = 2;							//pacman's velocity
	//int** gameMap = nullptr;

	bool portal_detect(int portal_pos[2][2], int window_shift[2]); //whether in the portal and transfer
	bool CanMove(int dir, int** gameMap);		//whether the dir can move
	void update_position(int dir);				//update obj pos

public:
	int dir_waitfor = 2;						//pacman's desire dir
	
	void setPos(int x=37, int y=15);
	//~character();

	int getX();
	int getY();

	void move(int** map, int portal_pos[2][2], int win_shift[2]); //move pacman
};