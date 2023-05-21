#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

bool GameAuto::game_set() {
	for (int i_g = 0; i_g < 32; i_g ++) {
		for (int i_c = 0; i_c < 4; i_c++) {
			for (int i_p = 0; i_p < 4; i_p++) {
				for (int i_w = 1; i_w < 16; i_w += 2) {
					if((i_w & 1) == 1) Q_table[i_g][i_c][i_p][i_w][0] = -999;
					if ((i_w & 1) == 2) Q_table[i_g][i_c][i_p][i_w][1] = -999;
					if ((i_w & 1) == 4) Q_table[i_g][i_c][i_p][i_w][2] = -999;
					if ((i_w & 1) == 8) Q_table[i_g][i_c][i_p][i_w][3] = -999;
				}
			}
		}
	}
	srand((unsigned)time(NULL));
	return true;
}
double GameAuto::get_expected_max_score(int g, int c, int p, int w) {
	double s = -10000;
	for (int dir = 0; dir < 4; dir ++) {
		s = max(s, Q_table[g][c][p][w][dir]);
	}
	return s;
}

double GameAuto::game_go(int* p,int x, int y, double reward, int dir) {
	if (dir == 0) x += 1;
	if (dir == 1) y += 1;
	if (dir == 2) x -= 1;
	if (dir == 3) y -= 1;

	double tmp = get_expected_max_score(p[0], p[1], p[2], p[3]);
	return  reward + rate * tmp;
}

int GameAuto::choose_dir(int g, int c, int p, int w) {
	int op;	
	if (rand() % 101 > greedy) {
		op = rand() % 4;
	}
	else {
		double maxx = -10000;
		for (int i = 0; i < 4; i++) {
			if (maxx < Q_table[g][c][p][w][i]) {
				maxx = Q_table[g][c][p][w][i];
				op = i;
			}
		}
	}

	return op;
}

void GameAuto::train(int* p, int x, int y, double reward, double reward_e, int dir) {
	double reward_ = game_go(p, x, y, reward, dir);
	Q_table[p[0]][p[1]][p[2]][p[3]][dir] += lr * (reward_ - reward_e);
}