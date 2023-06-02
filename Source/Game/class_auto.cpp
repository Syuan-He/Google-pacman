#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>
#include <string>

int GameAuto::choose_dir(int g, int c, int p, int w, int d) {
	int op;
	if (rand() % 101 > 80) {
		op = rand() % 4;
	}
	else {
		double maxx = -10000;
		for (int i = 0; i < 4; i++) {
			if (maxx < Q_table[g][c][p][w][d][i]) {
				maxx = Q_table[g][c][p][w][d][i];
				op = i;
			}
		}
	}

	return op;
}

void GameAuto::load_matrix(string dir) {
	ifstream inputFile(dir);
	if (inputFile.is_open()) {
		int number;
		for (int i_0 = 0; i_0 < 16; i_0++) {
			for (int i_1 = 0; i_1 < 4; i_1++) {
				for (int i_2 = 0; i_2 < 4; i_2++) {
					for (int i_3 = 0; i_3 < 16; i_3++) {
						for (int i_4 = 0; i_4 < 4; i_4++) {
							for (int i_5 = 0; i_5 < 4; i_5++) {
								inputFile >>  number;
								Q_table[i_0][i_1][i_2][i_3][i_4][i_5] = number;
							}
						}
					}
				}
			}
		}
		inputFile.close();
	}
}
