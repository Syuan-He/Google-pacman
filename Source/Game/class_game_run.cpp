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
}

//Debug顯示
void CGameStateRun::debugText(GamePacman P) {
	CDC *pDC = CDDraw::GetBackCDC();
	string strPacPos = "", strPacPoi = "";

	//地圖陣列位置
	strPacPos += to_string(P[0]) + ", " + to_string(P[1]);	//position in array
	//視窗位置
	strPacPoi += to_string(P.GetLeft()) + ", " + to_string(P.GetTop());

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 270 + 100, strPacPos);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 300 + 100, strPacPoi);

	CDDraw::ReleaseBackCDC();
}

