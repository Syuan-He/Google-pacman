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
#include <stdlib.h>

using namespace game_framework;

//根據階段顯示物件
void CGameStateRun::show_obj_by_phase() {
	//背景顯示
	Map.Background.ShowBitmap(2);
	//顯示傳送門
	Map.show_portal();
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
			//播放音效
			Game_audio -> Play(AUDIO_MOVE, true);
			phase = 1;
			modePlayTime = time(NULL);
			modeCount = 0;
			modeLock = false;
			ghostCatchCount = 0;
			preGhostCatchCount = 0;
		}
	}
	//階段1(遊戲中)
	else if (phase == 1) {
		//顯示鬼
		for (GameGhost &obj : ghosts) {
			obj.ShowBitmap(2);
		}
		if (Boss.get_is_using()) {
			Boss.ShowBitmap();
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
			//暫停音效
			Game_audio->Pause_one(AUDIO_MOVE);
			Game_audio->Stop(AUDIO_POWERUP);
			Sleep(300);
			//播放音效
			Game_audio->Play(AUDIO_DIE);
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
			initialGhosts();
		}
	}
	//階段3(生命歸零)
	else if (phase == 3) {
		score_his.push_back(Score.get_score());
		//change_level(end_level);
		phase = 5;
		exc_time_begin = time(NULL);
	}
	//階段4(吃完豆子)
	else if (phase == 4) {
		score_his.push_back(Score.get_score());

		if (Score.get_coin_nums() == 0) {
			//停止所有音效
			Game_audio->Stop(AUDIO_MOVE);
			Game_audio->Stop(AUDIO_POWERUP);
			//播放勝利
			Game_audio->Play(AUDIO_INTERMISSION);
			Score.set_coin_nums(1, 1);
		}
		Pacman.ShowBitmap(2);

		//遊戲結束
		if (time(NULL) - exc_time_begin > 5) {
			level ++;
			if (level < end_level) {
				change_level(level);
				Score.initialize(Map);
				Pacman.initialize();
				Pacman.hearts_icon.set_nums(2, 1);
				Pacman.heart_initialize();
				initialGhosts();
				Ready_icon.SetTopLeft(Ready_icon.window_shift[0], Ready_icon.window_shift[1]);
				exc_time_begin = time(NULL);
				phase = 0;
				Game_audio->Play(AUDIO_BEGIN);
			}
			else {
				//change_level(end_level);
				phase = 5;
				exc_time_begin = time(NULL);
			}
		}
	}
	//結算
	else if (phase == 5) {
	int Total = 0;
		CDC *pDC = CDDraw::GetBackCDC();
		string strConclusion = "RESULT";
		string strLine = "______________________________";
		string strScore = "Score: ";
		for (unsigned int i = 0; i < score_his.size(); i ++) {
			if(i < unsigned int((time(NULL) - exc_time_begin) / 2)){
				strScore += to_string(score_his[i]);
			}
			else {
				strScore += to_string(rand() % 10000);
			}

			if (i < score_his.size() - 1) {
				strScore += " + ";
			}
			Total += score_his[i];
		}
		CTextDraw::ChangeFontLog(pDC, 30, "微軟正黑體", RGB(255, 255, 255));
		CTextDraw::Print(pDC, 400, 150, strConclusion);
		CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
		CTextDraw::Print(pDC, 200, 180, strLine);
		CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
		CTextDraw::Print(pDC, 200, 220, strScore);

		if (time(NULL) - exc_time_begin > score_his.size() * 2) {
			CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
			CTextDraw::Print(pDC, 200, 300, strLine);
		}
		if (time(NULL) - exc_time_begin > score_his.size() * 2 + 1) {
			CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
			CTextDraw::Print(pDC, 200, 350, "Total: " + to_string(Total));
		}


		CDDraw::ReleaseBackCDC();
		
		if (time(NULL) - exc_time_begin > score_his.size() * 2 + 3) {
			change_level(level = 0);
			Score.initialize(Map);
			Pacman.initialize();
			Pacman.hearts_icon.set_nums(2, 1);
			Pacman.heart_initialize();
			initialGhosts();
			Ready_icon.SetTopLeft(Ready_icon.window_shift[0], Ready_icon.window_shift[1]);
			exc_time_begin = time(NULL);
			phase = 0;
			score_his.clear();
			GotoGameState(GAME_STATE_INIT);
		}
	}
}

//pacman是否被鬼抓到
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

		if (get_catch && !obj.isChoas && !invincible) {
			Pacman.hearts_icon.set_nums(-1);
			phase = 2;
			Pacman.SetFrameIndexOfBitmap(8);
			break;
		}
		else if (get_catch && obj.isChoas == 1) {
			Game_audio->Play(AUDIO_EAT_GHOST);
			obj.SetFrameIndexOfBitmap(16 + ghostCatchCount);

			obj.isChoas = 2;
			obj.choasFlash = false;
			obj.setVelocity(2);
			obj.update_moving_schedule();	
			Score.get_ghost(Pacman, obj, ghostCatchCount);
			ghostCatchCount++;
			if (ghostCatchCount > 3) {
				ghostCatchCount = 0;
			}
		}
	}

	if (Boss.get_is_using() && Pacman.IsOverlap(Pacman, Boss)&& !invincible) {
		Pacman.hearts_icon.set_nums(-1);
		phase = 2;
		Pacman.SetFrameIndexOfBitmap(8);
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
	for (GameGhost &obj : ghosts) {
		obj.set_game_map(Map);
	}

	Boss.set_game_map(Map);
	Score.set_game_map(Map);

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
	ghosts[0].set_inital(map_t["A_Blinky"].first, map_t["A_Blinky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	ghosts[1].set_inital(map_t["A_Pinky"].first, map_t["A_Pinky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	ghosts[2].set_inital(map_t["A_Inky"].first, map_t["A_Inky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	ghosts[3].set_inital(map_t["A_Clyde"].first, map_t["A_Clyde"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);

	Boss.set_is_using(false);
	if (map_t.count("A_Boss") != 0) {
		Boss.set_inital(map_t["A_Boss"].first, map_t["A_Boss"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
		Boss.set_is_using(true);
	}

	Pacman.hearts_icon.window_shift.set_value(map_t["W_Hearts"].first, map_t["W_Hearts"].second);
	Map.Background.window_shift.set_value(map_t["W_Background"].first, map_t["W_Background"].second);
	Ready_icon.window_shift.set_value(map_t["W_Ready"].first, map_t["W_Ready"].second);
	Score.set_window_shift(map_t["W_ScoreDot"].first, map_t["W_ScoreDot"].second);

	Map.Background.SetTopLeft(Map.Background.window_shift[0], Map.Background.window_shift[1]);
	Map.Background.SetFrameIndexOfBitmap(0);
}

//Debug顯示
void CGameStateRun::debugText() {
	CDC *pDC = CDDraw::GetBackCDC();

	string strPacPos = "", strPacPoi = "", strCatchTime = "", strInkyChoas = "", strInvincible = "Invincible: ";

	//地圖陣列位置
	strPacPos += to_string(Pacman[0]) + ", " + to_string(Pacman[1]);	//position in array
	//視窗位置
	strPacPoi += to_string(ghosts[2].GetLeft()) + ", " + to_string(ghosts[2].GetTop());

	strCatchTime += to_string(16*(ghosts[2].getInitPos(0)) - 4) + ", " + to_string(16*(ghosts[2].getInitPos(1)+1)+96);

	strInkyChoas += to_string(Inky.isChoas);

	strInvincible += invincible ? "True" : "False";

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 430, strPacPos);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 460, strPacPoi);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 490, strCatchTime);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 25, 520, strInkyChoas);

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 325, 430, strInvincible);

	CDDraw::ReleaseBackCDC();
}
