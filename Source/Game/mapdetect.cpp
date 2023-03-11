#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

//if object change the direction , will it hit the wall
//return: true(it won't), false(it will)
bool CGameStateRun::objCanMove(int dir, int x, int y) {
	switch (dir)
	{
	case 0:
		if (gameMap[y][x + 1] != 0) {
			return false;
		}
		else {
			return true;
		}
	case 1:
		if (gameMap[y - 1][x] != 0) {
			return false;
		}
		else {
			return true;
		}
	case 2:
		if (gameMap[y][x - 1] != 0) {
			return false;
		}
		else {
			return true;
		}
	case 3:
		if (gameMap[y + 1][x] != 0) {
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

//update object's position by the input direction 
void CGameStateRun::update_position(int dir, int* pos) {
	switch (dir)
	{
	case 0:
		pos[0] ++;
		break;
	case 1:
		pos[1] --;
		break;
	case 2:
		pos[0] --;
		break;
	case 3:
		pos[1] ++;
		break;
	default:
		// warning wrong dir
		break;
	}
}

//portal dectect and sending object to the other side
//return: true(object hit the portal), false(object didn't hit the portal)
bool CGameStateRun::portal_detect(CMovingBitmap* obj, int* pos, int portal_pos[2][2]) {
	if (pos[0] == portal_pos[0][0] && pos[1] == portal_pos[0][1]) {
		pos[0] = portal_pos[1][0];
		pos[1] = portal_pos[1][1];
		obj -> SetTopLeft(16 * (pos[0] - 2) + 10, 16 * (pos[1] - 1) + 10);
		return true;
	}
	else if (pos[0] == portal_pos[1][0] && pos[1] == portal_pos[1][1]) {
		pos[0] = portal_pos[0][0];
		pos[1] = portal_pos[0][1];
		obj -> SetTopLeft(16 * (pos[0] - 2) + 10, 16 * (pos[01] - 1) + 10);
		return true;
	}
	return false;
}