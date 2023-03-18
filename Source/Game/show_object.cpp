#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

void CGameStateRun::show_image_by_phase() {
	background.ShowBitmap(2); //show background
	pacman.ShowBitmap(2); //show pacman
	 
	//show coins
	for (CMovingBitmap t : coins) {
		t.ShowBitmap();
	}
	//show power pellet
	for (CMovingBitmap t : power_pellets) {
		t.ShowBitmap();
	}
}