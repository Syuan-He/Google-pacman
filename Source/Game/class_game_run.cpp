#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>
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
		GotoGameState(GAME_STATE_OVER);
	}
	//階段4(吃完豆子)
	else if (phase == 4) {
		Pacman.ShowBitmap(2);

		//遊戲結束
		if (time(NULL) - exc_time_begin > 4) {
			change_level(++ level);
			Score.initialize(Map);
			Pacman.initialize();
			Blinky.initialize();
			Pinky.initialize();
			Inky.initialize();
			Clyde.initialize();
			Ready_icon.SetTopLeft(Ready_icon.window_shift[0], Ready_icon.window_shift[1]);
			Pacman.hearts_icon.set_nums(2, 1);
			exc_time_begin = time(NULL);
			phase = 0;
		}
	}
}

//pacman是否被鬼抓到
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
		if (get_catch && !obj.isChoas) {
			Pacman.hearts_icon.set_nums(-1);
			phase = 2;
			Pacman.SetFrameIndexOfBitmap(8);
			break;
		}
	}
}

//切換關卡
void CGameStateRun::change_level(int level) {
	//地圖陣列初始化
	string str = "Resources/GameMap/GameMap_" + to_string(level);
	GameMap NewMap;
	NewMap.map_loader(str);
	Map = NewMap;

	//加入參考地圖
	Pacman.set_game_map(Map);
	Blinky.set_game_map(Map);
	Pinky.set_game_map(Map);
	Inky.set_game_map(Map);
	Clyde.set_game_map(Map);

	ifstream infile(str + "/charater_pos.txt");  // 打開文件
	map<string, pair<int, int>> map_t;  // 定義一個 map

	std::string key;
	int value_0, value_1;
	while (infile >> key >> value_0 >> value_1) {  // 逐行讀取文件中的值
		map_t[key] = pair<int, int>(value_0, value_1);  // 將值存入 map 中
	}

	infile.close();
	// 遍歷 map，輸出所有的 key 和 value
	Pacman.set_inital(map_t["A_Pacman"].first, map_t["A_Pacman"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	Blinky.set_inital(map_t["A_Blinky"].first, map_t["A_Blinky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	Pinky.set_inital(map_t["A_Pinky"].first, map_t["A_Pinky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	Inky.set_inital(map_t["A_Inky"].first, map_t["A_Inky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	Clyde.set_inital(map_t["A_Clyde"].first, map_t["A_Clyde"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);

	Map.Background.window_shift.set_value(map_t["W_Background"].first, map_t["W_Background"].second);
	Ready_icon.window_shift.set_value(map_t["W_Ready"].first, map_t["W_Ready"].second);
	Score.set_window_shift(map_t["W_ScoreDot"].first, map_t["W_ScoreDot"].second);

	Map.Background.SetTopLeft(Map.Background.window_shift[0], Map.Background.window_shift[1]);
	Map.Background.SetFrameIndexOfBitmap(0);
}

//Debug顯示
void CGameStateRun::debugText() {
	CDC *pDC = CDDraw::GetBackCDC();
	string strPacPos = "", strPacPoi = "", strGameTime = "", strInvincible = "Invincible: ";

	//地圖陣列位置
	strPacPos += to_string(Pacman[0]) + ", " + to_string(Pacman[1]);	//position in array
	//視窗位置
	strPacPoi += to_string(modeCount);

	strGameTime += to_string((time(NULL) - modePlayTime)%27);

	strInvincible += invincible ? "True" : "False";

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 430, strPacPos);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 460, strPacPoi);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 490, strGameTime);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 520, strInvincible);

	CDDraw::ReleaseBackCDC();
}