#pragma once

#include "../Library/gameutil.h"

using namespace game_framework;

class EnvFeedBack {
public:
	int	ghost_dis;
	int ghost_dir;
	int ghost_state;
	int power_dir;
	int coin_dir;
	int wall_dir;
};

class GameAuto {
private:
	//ghost_dis,ghost_dir,ghost_state
	//power_dir
	//coin_dir
	//wall_dir
	//choose_dir
	double Q_table[2][4][2][4][4][16][4] = {};

	double rate = 0.8;
	double lr = 0.01;
	int greedy = 70;
public:
	bool game_set();
	double get_expected_max_score(EnvFeedBack state);
	double game_go(EnvFeedBack state, double reward);
	int choose_dir(EnvFeedBack state);
	int choose_dir_By_Qtable(EnvFeedBack state);
	void train(EnvFeedBack state, int dir, double reward, double reward_e);
	
	void store_matrix(string dir);
	void load_matrix(string dir);
	bool create_Qtable(string dir);
};