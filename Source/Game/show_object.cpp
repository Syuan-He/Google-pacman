#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

//show object
void CGameStateRun::show_image_by_phase() {
	background.ShowBitmap(2); //show background
	pacman.ShowBitmap(2); //show pacman
	P1_icon.ShowBitmap(2);
	for (int i = 0; i < heart_num; i ++) {
		hearts_icon[i].ShowBitmap(2);
	}

	if (phase == 0) {
		time_t t = time(NULL);
		Ready_icon.ShowBitmap(2);
		if (t - exc_time_begin > 5) {
			heart_num--;
			phase = 1;
		}
	}
	else if (phase == 1) {
		//show coins
		for (CMovingBitmap* t : coins) {
			if (t != nullptr) {
				t->ShowBitmap();
			}
		}
		//show power pellet
		for (CMovingBitmap t : power_pellets) {
			t.ShowBitmap();
		}

		update_score(score);
	}
	else if(phase == 2){

	}
}

//update score
void  CGameStateRun::update_score(int sc) {
	vector<int> s(score_digits, -1);
	int time = 0;

	while (sc > 0) {
		s[time] = sc % 10;
		sc /= 10;
		time ++;
	}
	for (int i : s) {
		if (i != -1) {
			game_scores[time - 1].SetFrameIndexOfBitmap(i);
			game_scores[time - 1].ShowBitmap(2);
		}
		else {
			break;
		}
		time--;
	}
}