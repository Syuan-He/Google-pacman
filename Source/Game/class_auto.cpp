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

bool GameAuto::game_set() {
	for (int i_g = 0; i_g < 16; i_g ++) {
		for (int i_c = 0; i_c < 4; i_c++) {
			for (int i_p = 0; i_p < 4; i_p++) {
				for (int i_w = 1; i_w < 16; i_w += 2) {
					for (int i_d = 0; i_d < 4; i_d++) {
						if ((i_w & 1) == 1) Q_table[i_g][i_c][i_p][i_w][i_d][0] = -999;
						if ((i_w & 2) == 2) Q_table[i_g][i_c][i_p][i_w][i_d][1] = -999;
						if ((i_w & 4) == 4) Q_table[i_g][i_c][i_p][i_w][i_d][2] = -999;
						if ((i_w & 8) == 8) Q_table[i_g][i_c][i_p][i_w][i_d][3] = -999;
						//if (i_g > 7) Q_table[i_g][i_c][i_p][i_w][i_d][int((i_g - 8) / 2)] = 10;
					}
				}
			}
		}
	}
	srand((unsigned)time(NULL));
	return true;
}
double GameAuto::get_expected_max_score(int g, int c, int p, int w, int d) {
	double s = -10000;
	for (int dir = 0; dir < 4; dir ++) {
		s = max(s, Q_table[g][c][p][w][d][dir]);
	}
	return s;
}

double GameAuto::game_go(int* p,int x, int y, double reward, int dir) {
	if (dir == 0) x += 1;
	if (dir == 1) y += 1;
	if (dir == 2) x -= 1;
	if (dir == 3) y -= 1;

	double tmp = get_expected_max_score(p[0], p[1], p[2], p[3], p[4]);
	return  reward + rate * tmp;
}

int GameAuto::choose_dir(int g, int c, int p, int w, int d) {
	int op;	
	if (rand() % 101 > greedy) {
		op = rand() % 4;
	}
	else {
		double maxx = -10000;
		for (int i = 0; i < 4; i++) {
			if (maxx <= Q_table[g][c][p][w][d][i]) {
				maxx = Q_table[g][c][p][w][d][i];
				op = i;
			}
		}
	}

	return op;
}

void GameAuto::train(int* p, int x, int y, double reward, double reward_e, int dir) {
	double reward_ = game_go(p, x, y, reward, dir);
	Q_table[p[0]][p[1]][p[2]][p[3]][p[4]][dir] += lr * (reward_ - reward_e);
}

double GameAuto::count_reward_r(int* p, int x, int y, double reward, int dir) {
	return game_go(p, x, y, reward, dir);
}

void GameAuto::mult_train(int** p, double reward, double* reward_e, int time) {
	for (int i = 0; i < time; i ++) {
		Q_table[p[i][0]][p[i][1]][p[i][2]][p[i][3]][p[i][4]][p[i][5]] += lr * (reward - reward_e[i]);
	}
}

string GameAuto::get_Qtable_dir() {
	return Qtable_dir;
}

void GameAuto::create_file(string dir, string file_name) {
	string baseName = file_name.substr(0, file_name.find_last_of('.'));
	string extension = file_name.substr(file_name.find_last_of('.'));

	string newFileName = file_name;
	int counter = 0;
	while (ifstream(dir + newFileName)) {
		newFileName = baseName + "_" + to_string(counter) + extension;
		counter++;
	}

	ofstream file(dir + newFileName);
	if (file.is_open()) {
		Qtable_dir = dir + newFileName;
		file.close();
	}
	return;
}

void GameAuto::store_matrix(string dir) {
	ofstream outputFile(dir, ios::trunc);
	if (outputFile.is_open()) {
		for (int i_0 = 0; i_0 < 16; i_0++) {
			for (int i_1 = 0; i_1 < 4; i_1++) {
				for (int i_2 = 0; i_2 < 4; i_2++) {
					for (int i_3 = 0; i_3 < 16; i_3++) {
						for (int i_4 = 0; i_4 < 4; i_4++) {
							for (int i_5 = 0; i_5 < 4; i_5++) {
								outputFile << Q_table[i_0][i_1][i_2][i_3][i_4][i_5] << " ";
							}
							outputFile << "\n";
						}
						outputFile << "\n";
					}
					outputFile << "\n";
				}
				outputFile << "\n";
			}
			outputFile << "\n";
		}
		outputFile.close();
	}
}

void GameAuto::load_matrix(string dir) {
	ifstream inputFile(dir);
	if (inputFile.is_open()) {
		double number;
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
