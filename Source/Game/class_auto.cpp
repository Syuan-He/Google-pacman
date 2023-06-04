#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

bool GameAuto::game_set() {
	//for (int i_g = 0; i_g < 16; i_g++) {
	//	for (int i_c = 0; i_c < 4; i_c++) {
	//		for (int i_p = 0; i_p < 4; i_p++) {
	//			for (int i_w = 1; i_w < 16; i_w += 2) {
	//				for (int i_d = 0; i_d < 4; i_d++) {
	//					if ((i_w & 1) == 1) Q_table[i_g][i_c][i_p][i_w][i_d][0] = -999;
	//					if ((i_w & 2) == 2) Q_table[i_g][i_c][i_p][i_w][i_d][1] = -999;
	//					if ((i_w & 4) == 4) Q_table[i_g][i_c][i_p][i_w][i_d][2] = -999;
	//					if ((i_w & 8) == 8) Q_table[i_g][i_c][i_p][i_w][i_d][3] = -999;
	//					//if (i_g > 7) Q_table[i_g][i_c][i_p][i_w][i_d][int((i_g - 8) / 2)] = 10;
	//				}
	//			}
	//		}
	//	}
	//}
	srand((unsigned)time(NULL));
	return true;
}
double GameAuto::get_expected_max_score(EnvFeedBack state) {
	double s = -10000;
	for (int dir = 0; dir < 4; dir++) {
		s = max(s, Q_table[state.ghost_dis][state.ghost_dir][state.ghost_state][state.power_dir][state.coin_dir][state.wall_dir][dir]);
	}
	return s;
}

double GameAuto::game_go(EnvFeedBack state, double reward) {
	double tmp = get_expected_max_score(state);
	return  reward + rate * tmp;
}

int GameAuto::choose_dir(EnvFeedBack state) {
	int op;
	if (rand() % 101 > greedy) {
		op = rand() % 4;
	}
	else {
		double maxx = -10000;
		for (int i = 0; i < 4; i++) {
			if (maxx <= Q_table[state.ghost_dis][state.ghost_dir][state.ghost_state][state.power_dir][state.coin_dir][state.wall_dir][i]) {
				maxx = Q_table[state.ghost_dis][state.ghost_dir][state.ghost_state][state.power_dir][state.coin_dir][state.wall_dir][i];
				op = i;
			}
		}
	}

	return op;
}

void GameAuto::train(EnvFeedBack state, int dir, double reward, double reward_e) {
	double reward_ = game_go(state, reward);
	Q_table[state.ghost_dis][state.ghost_dir][state.ghost_state][state.power_dir][state.coin_dir][state.wall_dir][dir] += lr * (reward_ - reward_e);
}

//double GameAuto::count_reward_r(int* p, int x, int y, double reward, int dir) {
//	return game_go(p, x, y, reward, dir);
//}