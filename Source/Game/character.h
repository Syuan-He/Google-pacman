﻿#pragma once

#include "../Library/gameutil.h"
#include "object.h"

using namespace game_framework;

class Character : public game_framework::CMovingBitmap {
protected:

	int dir_now = 2;				//目前移動方向
	int dir_waitfor = 2;			//期望移動方向
	int position[2];				//位置
	int total_step = 0;				//已移動步數
	int velocity = 6;				//移動速動(1 ~ 16 數字越小越快)
	int waitVelocity = 6;			//期望移動速動
	int moving_schedule[16] = { 0 };	//移動距離表
	int initial_pos[2];				//起始位置
	int initial_frame_index;		//起始圖片
	//參考地圖
	GameMap gameMap;
public:
	//Character() {};
	//~Character() {};

	//位移
	TwoEleContainer window_shift{ 19, 96 };

	void update_position(int dir);
	void update_moving_schedule();
	bool CanMove(int dir);

	void set_dir_waitfor(int dir);
	void set_dir(int dir);
	void setPos(int x, int y);
	void setVelocity(int v);
	void set_game_map(const GameMap& map_t);
	void set_inital(int x, int y, int w_x, int w_y, int index);
	void initialize();

	int get_velocity();
	//回傳pacman位置
	int& operator[](int index);
};

//Pacman
class GamePacman : public Character {
private:
	int step_counter = 0;
	bool stack = false;
public:
	//GamePacman() {};
	//~GamePacman() {};

	void move();
	//血條
	MultUIObj hearts_icon{ 2 , 25, 388 };

	void reset_step_counter();
	
	int get_step_counter();
	void heart_initialize();
	void show_heart_icon(int size);
	int getDirNow();
	int get_total_step();
	bool get_stack();
};

//鬼
class GameGhost : public Character {
private:
	bool setDirLock;				//防止turnBack()無法生效
public:
	void initialize();
	void inHomeAnim();
	void outDoorAnim();
	
	int isChoas = 0;				// isChoas 0: 狀態無; 1: 混亂; 2: 回家 3: 在家裡
	bool choasFlash = false;		// 設定 Choas 快結束時的閃爍
	bool inHome = true;

	void move(int x1, int y1);

	int selectDir(int dir, int x1, int y1);
	void turnBack();

	bool isVaildNode(int x, int y, int xx, int yy);
	int astar(int x0, int y0, int x1, int y1);

	int getInitPos(int n);
};
