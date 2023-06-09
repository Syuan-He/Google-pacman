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
		phase = 1;
		modePlayTime = time(NULL);
		modeCount = 0;
		modeLock = false;
		ghostCatchCount = 0;
		preGhostCatchCount = 0;
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
		//重新初始化
		exc_time_begin = time(NULL);
		phase = 0;
		Pacman.initialize();

		int x, y, dir;
		do {
			x = rand() % 62;
			y = rand() % 17;
		} while (Map[y][x] != 0);
		Pacman.setPos(x, y);
		do {
			dir = rand() % 4;
			if (dir == 0 && Map[y][x + 1] == 0) {
				Pacman.set_dir_waitfor(0);
				break;
			}
			else if (dir == 1 && Map[y - 1][x] == 0) {
				Pacman.set_dir_waitfor(1);
				break;
			}
			else if (dir == 2 && Map[y][x - 1] == 0) {
				Pacman.set_dir_waitfor(2);
				break;
			}
			else if (dir == 3 && Map[y + 1][x] == 0) {
				Pacman.set_dir_waitfor(3);
				break;
			}
		} while (true);
		
		initialGhosts();
		Boss.initialize();
		Score.initialize(Map);
		Score.set_game_map(Map);

		accuracy();
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
		if (Score.get_coin_nums() == 0) {
			//停止所有音效
			//播放勝利
			Score.set_coin_nums(1, 1);
		}
		Pacman.ShowBitmap(2);

		//遊戲結束
		if (time(NULL) - exc_time_begin > 5) {
			score_his.push_back(Score.get_score());
			level ++;
			if (level < end_level) {
				change_level(level);
				
				Score.initialize(Map);
				Pacman.initialize();
				Pacman.hearts_icon.set_nums(2, 1);
				Pacman.heart_initialize();
				initialGhosts();
				Boss.initialize();
				Ready_icon.SetTopLeft(Ready_icon.window_shift[0], Ready_icon.window_shift[1]);
				
				exc_time_begin = time(NULL);
				phase = 0;
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
		CTextDraw::Print(pDC, 100, 220, strScore);

		if (time(NULL) - exc_time_begin > score_his.size() * 2) {
			CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
			CTextDraw::Print(pDC, 200, 300, strLine);
		}
		if (time(NULL) - exc_time_begin > score_his.size() * 2 + 1) {
			CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));
			CTextDraw::Print(pDC, 112, 350, "Total: " + to_string(Total));
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
			phase = 2;
			Pacman.SetFrameIndexOfBitmap(8);

			Reward += R_ate_by_ghost;
			break;
		}
		else if (get_catch && obj.isChoas == 1) {
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

			Reward += R_eat_ghost;
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
	Score.set_game_map(Map);
	Pacman.set_game_map(Map);
	for (GameGhost &obj : ghosts) {
		obj.set_game_map(Map);
	}
	Boss.set_game_map(Map);

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
		Boss.window_shift.set_value(map_t["W_Boss"].first, map_t["W_Boss"].second);
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

	string strPacPos = "", strPacPoi = "", strCatchTime = "", strInkyChoas = "", strInvincible = "Invincible: ", strNearGhost = "D(ghost):",
		strNearGhostDir = "Dir(ghost):", strNearGhostState = "S(ghost):", strNearCoin = "Dir(coin):", strNearPower = "Dir(power):", strAccuracy = "A:";

	//地圖陣列位置
	strPacPos += to_string(Pacman[0]) + ", " + to_string(Pacman[1]);	//position in array
	//視窗位置
	strPacPoi += to_string(ghosts[2].GetLeft()) + ", " + to_string(ghosts[2].GetTop());

	strCatchTime += to_string(16*(ghosts[2].getInitPos(0)) - 4) + ", " + to_string(16*(ghosts[2].getInitPos(1)+1)+96);

	strInkyChoas += to_string(Inky.isChoas);

	strInvincible += invincible ? "True" : "False";

	int xx = Pacman[0];
	int yy = Pacman[1];

	if (training) {
		strNearGhost += to_string(min_dis_pacman_ghost(xx, yy).first.first);
		strNearGhostDir += to_string(min_dis_pacman_ghost(xx, yy).first.second);
		strNearGhostState += to_string(min_dis_pacman_ghost(xx, yy).second);
		strNearCoin += to_string(near_coin_dir(xx, yy));
		strNearPower += to_string(near_power_dir(xx, yy));
		strAccuracy += to_string(last_accuracy);
	}
	

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
	CTextDraw::Print(pDC, 625, 490, strAccuracy);
	CTextDraw::ChangeFontLog(pDC, 24, "微軟正黑體", RGB(255, 255, 255));

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
			dir = t.getAstar(x_p, y_p, x_g, y_g);
		}
	}
	min_dis = min_dis > DIS_NEAR ? 1 : 0;
	return pair<pair<int, int>, int>(pair<int, int>(min_dis, dir), is_choas);
}

int CGameStateRun::near_coin_dir(int x, int y) {
	return Score.get_coin_dir(x, y);
}

int CGameStateRun::near_power_dir(int x, int y) {
	return Score.get_power_dir(16 * x + Pacman.window_shift[0] + 6, 16 * y + Pacman.window_shift[1] + 4);
}

int CGameStateRun::near_wall(int x, int y) {
	int res = 0;
	res += Map[y][x + 1] == 1? 1:0;
	res += Map[y - 1][x] == 1? 2:0;
	res += Map[y][x - 1] == 1? 4:0;
	res += Map[y + 1][x] == 1? 8:0;

	return res;
}

EnvFeedBack CGameStateRun::expect_next_step(int dir) {
	int dis;
	int x = Pacman[0];
	int y = Pacman[1];

	if (dir == 0) x++;
	else if (dir == 1) y--;
	else if (dir == 2) x--;
	else y++;

	pair<pair<int, int>, int> a = min_dis_pacman_ghost(x, y);
	int b = near_coin_dir(x, y);
	int c = near_power_dir(x, y);
	int d = near_wall(x, y);

	if (a.first.first < DIS_NEAR) dis = 0;
	else dis = 1;

	EnvFeedBack t;

	t.ghost_dis = dis;
	t.ghost_dir = a.first.second;
	t.ghost_state = a.second;
	t.power_dir = c;
	t.coin_dir = b;
	t.wall_dir = d;

	return t;
}

void CGameStateRun::accuracy() {
	dead_time --;
	if (dead_time == 0) {
		last_accuracy = eaten_coin_num / double(100 * total_coin_num);
		dead_time = 100;
		eaten_coin_num = 0;
	}
}