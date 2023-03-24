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

	for (CMovingBitmap t : hearts_icon) {
		t.ShowBitmap(2);
	}
	 
	//show coins
	for (CMovingBitmap* t : coins) {
		if (t != nullptr) {
			t -> ShowBitmap();
		}
	}
	//show power pellet
	for (CMovingBitmap t : power_pellets) {
		t.ShowBitmap();
	}

	update_score(score);
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