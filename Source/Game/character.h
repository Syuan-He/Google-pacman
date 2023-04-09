#pragma once

#include "../Library/gameutil.h"
#include "object.h"

using namespace game_framework;

class Character : public game_framework::CMovingBitmap {
protected:

	int dir_now = 2;				//目前移動方向
	int dir_waitfor = 2;			//期望移動方向
	int position[2];				//位置
	int total_step = 0;				//移動步數(最大16)
	int velocity = 2;				//移動速動
	int initial_pos[2];				//起始位置
	int initial_frame_index;

	//參考地圖
	GameMap gameMap;
public:
	Character() {};
	~Character() {};

	//位移
	TwoEleContainer window_shift{ 19, 96 };

	void move();
	void update_position(int dir);
	bool CanMove(int dir);

	void set_dir_waitfor(int dir);
	void setPos(int x, int y);
	void set_game_map(const GameMap& map_t);
	void set_inital(int x, int y, int index);
	void initialize();

	//回傳pacman位置
	int& operator[](int index);
};

//Pacman
class GamePacman : public Character {
public:
	GamePacman() {};
	~GamePacman() {};

	//血條
	MultUIObj hearts_icon{ 2 , 25, 388 };
	//位移
	TwoEleContainer window_shift{ 19, 96 };

	void show_heart_icon(int size);

	int getDirNow();
};

//鬼
class GameGhost : public Character {
public:
	bool isVaildNode(int x, int y, int xx, int yy);
	int astar(int x0, int y0, int x1, int y1);
	void move(int x1, int y1);
	int selectDir(int dir, int x1, int y1);
	int getDirWait();
};
