#pragma once

#include "../Library/gameutil.h"

using namespace game_framework;

class Pacman : public CMovingBitmap
{
private:
	//CMovingBitmap pacman; //pacman����
	int dir_now = 2; //pacman�����ʤ�V
	int dir_waitfor = 2; //pacman���檺���ʤ�V
	int position[2] = { 37, 15 }; //pacman����l�y��
	int total_step = 0; //pacman�������`�B��(�̤j16(2����l�a�ϤU���@�B��))
	int velocity = 2; //pacman�����ʳt��
	int** gameMap = nullptr;
	
public:
	Pacman();
	~Pacman();

	void move(); //����pacman
	void update_position(int dir); //��s���w����y��
	bool CanMove(int dir); //�O�_�i�H�V���w��V����
	bool portal_detect(); //�����O�_�i�J���e���ò��ʫ��w����
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