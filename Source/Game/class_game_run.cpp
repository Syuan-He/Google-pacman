#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <time.h>
#include <string>

using namespace game_framework;

//根據階段顯示物件
void CGameStateRun::show_obj_by_phase() {
	//背景顯示
	Background.ShowBitmap(2);
	//豆子顯示
	Score.show_coins();
	//大力丸顯示
	Score.show_power_pellets();
	//血條顯示
	Pacman.show_heart_icon(2);
	//P1顯示
	P1_icon.ShowBitmap(2);
	//階段0
	if (phase == 0) {
		//pacman顯示
		Pacman.ShowBitmap(2);
		//Ready圖標顯示
		Ready_icon.ShowBitmap(2);
		//5秒後進入階段2
		if (time(NULL) - exc_time_begin > 4) {
			phase = 1;
		}
	}
	//階段1(遊戲中)
	else if (phase == 1) {
		//pacman顯示
		Pacman.ShowBitmap(2);
		//顯示分數
		Score.show_score(2);
		//顯示鬼
		Blinky.ShowBitmap(2);
		Pinky.ShowBitmap(2);
		Inky.ShowBitmap(2);
		Clyde.ShowBitmap(2);
	}
	//階段2(碰到鬼)
	else if (phase == 2) {
		//死亡動畫
		if (Pacman.GetFrameIndexOfBitmap() == 8) {
			Sleep(300);
		}
		Pacman.SetFrameIndexOfBitmap(Pacman.GetFrameIndexOfBitmap() + 1);
		Pacman.ShowBitmap(2);
		Sleep(130);

		if (Pacman.GetFrameIndexOfBitmap() == 20) {
			Sleep(500);
			if (Pacman.hearts_icon.get_nums() < 0) {
				phase = 3;
			}
			else {
				exc_time_begin = time(NULL);
				phase = 0;
			}

			//重新初始化
			Pacman.initialize();
			Blinky.initialize();
			Pinky.initialize();
			Inky.initialize();
			Clyde.initialize();
		}
	}
	//階段3(生命歸零)
	else if (phase == 3) {
		Ready_icon.SetFrameIndexOfBitmap(1);
		Ready_icon.ShowBitmap(2);
		//phase = 0; //之後替換為遊戲結束
	}
	//階段4(吃完豆子)
	else if (phase == 4) {
		if (Score.get_coin_nums() == 0) {
			Pacman.SetFrameIndexOfBitmap(0);
			Background.SetAnimation(300, false);
			exc_time_begin = time(NULL);
			Score.set_coin_nums(-1);
		}

		Pacman.ShowBitmap(2);

		//遊戲結束
		if (time(NULL) - exc_time_begin > 4) {
			phase = 0; //之後替換為遊戲結束
		}
	}
}

void CGameStateRun::pacman_get_catch(int mode) {
	if (phase != 1) {
		return;
	}
	vector<GameGhost> ghosts = { Blinky, Pinky, Inky, Clyde };
	for (GameGhost obj : ghosts) {
		bool get_catch = false;
		if (mode == 0) {
			get_catch = Pacman.IsOverlap(Pacman, obj);
		}
		else if (mode == 1) {
			get_catch = (Pacman[0] == obj[0] && Pacman[1] == obj[1]);
		}
		if (get_catch) {
			Pacman.hearts_icon.set_nums(-1);
			phase = 2;
			Pacman.SetFrameIndexOfBitmap(8);
			break;
		}
	}
}

void obj_initialization() {
	
}

//Debug顯示
void CGameStateRun::debugText() {
	CDC *pDC = CDDraw::GetBackCDC();
	string strPacPos = "", strPacPoi = "", strGhostDirWait = "";

	//地圖陣列位置
	strPacPos += to_string(Pacman[0]) + ", " + to_string(Pacman[1]);	//position in array
	//視窗位置
	strPacPoi += to_string(Pacman.GetLeft()) + ", " + to_string(Pacman.GetTop());

	strGhostDirWait += to_string(Blinky.getDirWait());

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 270 + 100, strPacPos);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 300 + 100, strPacPoi);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 330 + 100, strGhostDirWait);

	CDDraw::ReleaseBackCDC();
}

//畢氏定理距離
//*
float CGameStateRun::pythagorean(int x, int y, int x1, int y1) {
	return sqrt((float)((x - x1)*(x - x1) + (y - y1)*(y - y1)));
}//*/