﻿#include "stdafx.h"
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
	Map.Background.ShowBitmap(2);
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
			modePlayTime = time(NULL);
			modeCount = 0;
			modeLock = false;
		}
	}
	//階段1(遊戲中)
	else if (phase == 1) {
		//顯示鬼
		Blinky.ShowBitmap(2);
		Pinky.ShowBitmap(2);
		Inky.ShowBitmap(2);
		Clyde.ShowBitmap(2);
		//pacman顯示
		Pacman.ShowBitmap(2);
		//顯示分數
		Score.show_score(2);
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
			Map.Background.SetAnimation(300, false);
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
	bool get_catch = false;
	int count = 0;
	for (GameGhost obj : ghosts) {
		get_catch = false;
		count++;
		if (mode == 0) {
			get_catch = Pacman.IsOverlap(Pacman, obj);
		}
		else if (mode == 1) {
			get_catch = (Pacman[0] == obj[0] && Pacman[1] == obj[1]);
		}

		if (get_catch && !obj.isChoas) {
			Pacman.hearts_icon.set_nums(-1);
			phase = 2;
			Pacman.SetFrameIndexOfBitmap(8);
			break;
		}
		else if (get_catch) {
			switch (count) {
			case 1:
				if (Blinky.isChoas == 1) {
					Blinky.isChoas = 2;
					Blinky.choasFlash = false;
					Blinky.setVelocity(4);
					Score.get_ghost(Pacman, Blinky, ghostCatchTime);
				}
				break;
			case 2:
				if (Pinky.isChoas == 1) {
					Pinky.isChoas = 2;
					Pinky.choasFlash = false;
					Pinky.setVelocity(4);
					Score.get_ghost(Pacman, Pinky, ghostCatchTime);
				}
				break;
			case 3:
				if (Inky.isChoas == 1) {
					Inky.isChoas = 2;
					Inky.choasFlash = false;
					Inky.setVelocity(4);
					Score.get_ghost(Pacman, Inky, ghostCatchTime);
				}
				break;
			case 4:
				if (Clyde.isChoas == 1) {
					Clyde.isChoas = 2;
					Clyde.choasFlash = false;
					Clyde.setVelocity(4);
					Score.get_ghost(Pacman, Clyde, ghostCatchTime);
				}
				break;
			}
		}
	}
}

void obj_initialization() {
	
}

//Debug顯示
void CGameStateRun::debugText() {
	CDC *pDC = CDDraw::GetBackCDC();
	string strPacPos = "", strPacPoi = "", strCatchTime = "", strInkyChoas = "";

	//地圖陣列位置
	strPacPos += to_string(Pacman[0]) + ", " + to_string(Pacman[1]);	//position in array
	//視窗位置
	strPacPoi += to_string(modeCount);

	strCatchTime += to_string(Score.get_power(Pacman));

	strInkyChoas += to_string(Inky.isChoas);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 430, strPacPos);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 460, strPacPoi);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 490, strCatchTime);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 520, strInkyChoas);

	CDDraw::ReleaseBackCDC();
}