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
		for (GameGhost &obj : ghosts) {
			obj.ShowBitmap(2);
		}
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
			for (GameGhost &obj : ghosts) {
				obj.initialize();
			}
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
		if (Score.get_coin_nums() == 0) {
			Pacman.SetFrameIndexOfBitmap(0);
			Map.Background.SetAnimation(300, false);
			exc_time_begin = time(NULL);
			Score.set_coin_nums(-1);
		}

		Pacman.ShowBitmap(2);

		//遊戲結束
		if (time(NULL) - exc_time_begin > 4) {
			level += 1;
			change_level(level);
			Score.initialize(Map);
			Pacman.initialize();
			for (GameGhost &obj : ghosts) {
				obj.initialize();
			}
			Ready_icon.SetTopLeft(Ready_icon.window_shift[0], Ready_icon.window_shift[1]);
			Pacman.hearts_icon.set_num_abs(2);
			exc_time_begin = time(NULL);
			phase = 0;
		}
	}
}

void CGameStateRun::pacman_get_catch(int mode) {
	if (phase != 1) {
		return;
	}

	bool get_catch = false;
	for (GameGhost &obj : ghosts) {
		get_catch = false;
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
		else if (get_catch && obj.isChoas == 1) {
					obj.isChoas = 2;
					obj.choasFlash = false;
					obj.setVelocity(4);
					Score.get_ghost(Pacman, obj, ghostCatchTime);	
		}
	}
}

void CGameStateRun::change_level(int level) {
	//地圖陣列初始化
	string str = "Resources/GameMap/GameMap_" + to_string(level);
	GameMap NewMap;
	NewMap.map_loader(str);
	Map = NewMap;
	Map.Background.SetTopLeft(Map.Background.window_shift[0], Map.Background.window_shift[1]);
	Map.Background.SetFrameIndexOfBitmap(0);

	//加入參考地圖
	Pacman.set_game_map(Map);
	for (GameGhost &obj : ghosts) {
		obj.set_game_map(Map);
	}

	ifstream infile(str + "/charater_pos.txt");  // 打開文件
	map<string, pair<int, int>> map_t;  // 定義一個 map

	std::string key;
	int value_0, value_1;
	while (infile >> key >> value_0 >> value_1) {  // 逐行讀取文件中的值
		map_t[key] = pair<int, int>(value_0, value_1);  // 將值存入 map 中
	}

	infile.close();
	// 遍歷 map，輸出所有的 key 和 value

	Pacman.set_inital(map_t["Pacman"].first, map_t["Pacman"].second, 0);
	ghosts[0].set_inital(map_t["Blinky"].first, map_t["Blinky"].second, 0);
	ghosts[1].set_inital(map_t["Pinky"].first, map_t["Pinky"].second, 0);
	ghosts[2].set_inital(map_t["Inky"].first, map_t["Inky"].second, 0);
	ghosts[3].set_inital(map_t["Clyde"].first, map_t["Clyde"].second, 0);

	Ready_icon.window_shift.set_value(map_t["Ready"].first, map_t["Ready"].second);
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
