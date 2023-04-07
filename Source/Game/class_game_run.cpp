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
	//pacman顯示
	Pacman.ShowBitmap(2);
	//豆子顯示
	Score.show_coins();
	//大力丸顯示
	Score.show_power_pellets();
	//血條顯示
	Pacman.show_heart_icon(2);
	//P1顯示
	P1_icon.ShowBitmap(2);
	//階段1
	if (phase == 0) {
		time_t t = time(NULL);
		//Ready圖標顯示
		Ready_icon.ShowBitmap(2);
		//5秒後進入階段2
		if (t - exc_time_begin > 5) {
			Pacman.hearts_icon.set_nums(-1);
			phase = 1;
		}
	}
	//階段2
	else if (phase == 1) {
		//show coins
		Score.show_coins();
		//show power pellet
		Score.show_power_pellets();
		//顯示分數
		Score.show_score(2);
	}
	else if (phase == 2) {

	}
	//顯示鬼
	Blinky.ShowBitmap(2);
	Pinky.ShowBitmap(2);
	Inky.ShowBitmap(2);
	Clyde.ShowBitmap(2);
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