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
	for (int i_g0 = 0; i_g0 < 2; i_g0 ++) {
		for (int i_g1 = 0; i_g1 < 4; i_g1++) {
			for (int i_g2 = 0; i_g2 < 2; i_g2++) {
				for (int i_c = 0; i_c < 4; i_c++) {
					for (int i_p = 0; i_p < 4; i_p++) {
						for (int i_w = 0; i_w < 16; i_w ++) {
							if ((i_w & 1) != 0) Q_table[i_g0][i_g1][i_g2][i_c][i_p][i_w][0] = -999;
							if ((i_w & 2) != 0) Q_table[i_g0][i_g1][i_g2][i_c][i_p][i_w][1] = -999;
							if ((i_w & 4) != 0) Q_table[i_g0][i_g1][i_g2][i_c][i_p][i_w][2] = -999;
							if ((i_w & 8) != 0) Q_table[i_g0][i_g1][i_g2][i_c][i_p][i_w][3] = -999;
						}
					}
				}
			}
		}
	}
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
	int op = rand() % 4;;
	if (rand() % 101 > greedy) {
		while (Q_table[state.ghost_dis][state.ghost_dir][state.ghost_state][state.power_dir][state.coin_dir][state.wall_dir][op] < -100) {
			op = rand() % 4;
		}
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