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

		phase = 1;
		modePlayTime = time(NULL);
		modeCount = 0;
		modeLock = false;
		ghostCatchTime = 0;
		flag = 0;
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
			if (using_auto) {
				round_time = (round_time + 1) % 10;
				total_score += Score.get_score() / 10;	
				if (round_time == 9) {
					accurcy = total_score / round_time / total_coin_nums;
					total_score = 0;
				}
			}
			else {
				////暫停音效
				//Game_audio->Pause_one(AUDIO_MOVE);
				//Game_audio->Stop(AUDIO_POWERUP);
				//Sleep(300);
				////播放音效
				//Game_audio->Play(AUDIO_DIE);
			}
		}
		Pacman.SetFrameIndexOfBitmap(Pacman.GetFrameIndexOfBitmap() + 1);
		Pacman.ShowBitmap(2);
		//Sleep(130);

		if (Pacman.GetFrameIndexOfBitmap() == 20) {
			//Sleep(500);
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
			if (using_auto) Score.initialize(Map);
		}
	}
	//階段3(生命歸零)
	else if (phase == 3) {
		change_level(level = 0);
		Score.initialize(Map);
		Pacman.initialize();
		Pacman.hearts_icon.set_nums(2, 1);
		Pacman.heart_initialize();
		initialGhosts();
		Ready_icon.SetTopLeft(Ready_icon.window_shift[0], Ready_icon.window_shift[1]);
		exc_time_begin = time(NULL);
		phase = 0;
		GotoGameState(GAME_STATE_INIT);
	}
	//階段4(吃完豆子)
	else if (phase == 4) {
		if (Score.get_coin_nums() == 0) {
			////停止所有音效
			//Game_audio->Stop(AUDIO_MOVE);
			//Game_audio->Stop(AUDIO_POWERUP);
			////播放勝利
			//Game_audio->Play(AUDIO_INTERMISSION);
			Score.set_coin_nums(1, 1);
		}
		Pacman.ShowBitmap(2);

		//遊戲結束
		if (time(NULL) - exc_time_begin > 5) {
			change_level(++ level);
			Score.initialize(Map);
			Pacman.initialize();
			Pacman.hearts_icon.set_nums(2, 1);
			Pacman.heart_initialize();
			initialGhosts();
			Ready_icon.SetTopLeft(Ready_icon.window_shift[0], Ready_icon.window_shift[1]);
			exc_time_begin = time(NULL);
			phase = 0;
			//Game_audio->Play(AUDIO_BEGIN);
		}
	}
}

//pacman是否被鬼抓到
int CGameStateRun::pacman_get_catch(int mode) {
	if (phase != 1) {
		return 0;
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
			if(!using_auto) Pacman.hearts_icon.set_nums(-1);
			phase = 2;
			Pacman.SetFrameIndexOfBitmap(8);
			return 1;
			break;
		}
		else if (get_catch && obj.isChoas == 1) {
			//Game_audio->Play(AUDIO_EAT_GHOST);
			obj.SetFrameIndexOfBitmap(16 + ghostCatchTime);

			obj.isChoas = 2;
			obj.choasFlash = false;
			obj.setVelocity(2);
			obj.update_moving_schedule();	
			Score.get_ghost(Pacman, obj, ghostCatchTime);
			ghostCatchTime++;
			if (ghostCatchTime > 3) {
				ghostCatchTime = 0;
			}
			return 2;
		}
	}
	return 0;
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
	Score.set_game_map(Map);
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
	Pacman.set_inital(map_t["A_Pacman"].first, map_t["A_Pacman"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	ghosts[0].set_inital(map_t["A_Blinky"].first, map_t["A_Blinky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	ghosts[1].set_inital(map_t["A_Pinky"].first, map_t["A_Pinky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	ghosts[2].set_inital(map_t["A_Inky"].first, map_t["A_Inky"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);
	ghosts[3].set_inital(map_t["A_Clyde"].first, map_t["A_Clyde"].second, map_t["W_Character"].first, map_t["W_Character"].second, 0);

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

	string strPacPos = "", strPacPoi = "", strCatchTime = "", strInkyChoas = "", strInvincible = "Invincible: ", strNearGhost = "D(ghost):",
		strNearGhostDir = "Dir(ghost):", strNearGhostState = "S(ghost):", strNearCoin = "Dir(coin):", strNearPower = "Dir(power):",
		strAccurcy = "A:", strQtable = "Qtable:";

	//地圖陣列位置
	strPacPos += to_string(Pacman[0]) + ", " + to_string(Pacman[1]);	//position in array
	//視窗位置
	strPacPoi += to_string(ghosts[2].GetLeft()) + ", " + to_string(ghosts[2].GetTop());

	strCatchTime += to_string(16*(ghosts[2].getInitPos(0)) - 4) + ", " + to_string(16*(ghosts[2].getInitPos(1)+1)+96);

	strInkyChoas += to_string(Inky.isChoas);

	strInvincible += invincible ? "True" : "False";

	int xx = Pacman[0];
	int yy = Pacman[1];
	strNearGhost += to_string(min_dis_pacman_ghost(xx, yy).first.first);
	strNearGhostDir += to_string(min_dis_pacman_ghost(xx, yy).first.second);
	strNearGhostState += to_string(min_dis_pacman_ghost(xx, yy).second);
	strNearCoin += to_string(near_coin_dir(xx, yy));
	strNearPower += to_string(near_power_dir(xx, yy));
	strAccurcy += to_string(accurcy);
	strQtable += to_string(using_Qtable);

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

	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 325, 460, strNearGhost);
	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 325, 490, strNearGhostState);
	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 325, 520, strNearCoin);
	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 625, 430, strNearPower);
	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 625, 460, strNearGhostDir);
	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 625, 490, strAccurcy);
	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 625, 520, strQtable);

	CDDraw::ReleaseBackCDC();
}

//test
pair<pair<int, int>, int> CGameStateRun::min_dis_pacman_ghost(int x_p, int y_p) {
	int min_dis = INT_FAST16_MAX;
	int is_choas = 0;
	int dir;

	for (GameGhost t : ghosts) {
		int x_g = t[0];
		int y_g = t[1];

		double dis = pow((x_g - x_p), 2) + pow((y_g - y_p), 2);
		if (min_dis > int(pow(dis, 0.5))) {
			min_dis = int(pow(dis, 0.5));
			is_choas = t.isChoas == 1 ? 1 : 0;
			if (abs(x_p - x_g) > abs(y_p - y_g)) {
				dir = x_p > x_g ? 2 : 0;
			}
			else {
				dir = y_p > y_g ? 1 : 3;
			}
		}
	}
	return pair<pair<int, int>, int>(pair<int, int>(min_dis, dir), is_choas);
}

int CGameStateRun::near_coin_dir(int x, int y) {
	return Score.get_coin_dir(16 * x + Pacman.window_shift[0] + 6, 16 * y + Pacman.window_shift[1] + 4, x, y);
}

int CGameStateRun::near_power_dir(int x, int y) {
	return Score.get_power_dir(16 * x + Pacman.window_shift[0] + 6, 16 * y + Pacman.window_shift[1] + 4);
}

pair<pair<int, int>, pair<int, int>> CGameStateRun::near_wall(int x, int y) {
	pair<pair<int, int>, pair<int, int>> t;
	t.first.first = Map[y][x + 1] == 1;
	t.second.first = Map[y - 1][x] == 1;
	t.first.second = Map[y][x - 1] == 1;
	t.second.second = Map[y + 1][x] == 1;

	return t;
}

int* CGameStateRun::expect_next_step(int dir) {
	int* t = new int[6];
	int dis;
	int x = Pacman[0];
	int y = Pacman[1];

	if (dir == 0) x ++;
	else if (dir == 1) y --;
	else if (dir == 2) x --;
	else y ++;

	pair<pair<int, int>, int> a = min_dis_pacman_ghost(x, y);
	int b = near_coin_dir(x, y);
	int c = near_power_dir(x, y);
	pair<pair<int, int>, pair<int, int>> d = near_wall(x, y);

	if (a.first.first < DIS_NEAR) dis = 0;
	else dis = 1;

	t[0] = dis + 2 * a.first.second + 8 * a.second;
	t[1] = b;
	t[2] = c;
	t[3] = d.first.first * 1 + d.first.second * 4 + d.second.first * 2 + d.second.second * 8;
	t[4] = Pacman.getDirNow();
	t[5] = dir;

	return t;
}

