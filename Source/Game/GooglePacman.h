#pragma once

#include "../Library/gameutil.h"

using namespace game_framework;

class Pacman : public CMovingBitmap
{
private:
	//CMovingBitmap pacman; //pacman物件
	int dir_now = 2; //pacman的移動方向
	int dir_waitfor = 2; //pacman期望的移動方向
	int position[2] = { 37, 15 }; //pacman的初始座標
	int total_step = 0; //pacman的移動總步數(最大16(2倍原始地圖下的一步長))
	int velocity = 2; //pacman的移動速度
	int** gameMap = nullptr;
	
public:
	Pacman();
	~Pacman();

	void move(); //移動pacman
	void update_position(int dir); //更新指定物件座標
	bool CanMove(int dir); //是否可以向指定方向移動
	bool portal_detect(); //偵測是否進入重送門並移動指定物件
};

class Ghost : public CMovingBitmap
{
private:
	
public:
	Ghost();
	~Ghost();

	bool isVaildNode(int x, int y, int xx, int yy);
	int astar(int x0, int y0, int x1, int y1);
};