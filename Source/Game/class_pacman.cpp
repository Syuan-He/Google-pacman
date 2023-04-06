#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace game_framework;

//血條顯示
void GamePacman::show_heart_icon(int size) {
	for (int i = 0; i < hearts_icon.get_nums(); i++) {
		hearts_icon[i].ShowBitmap(size);
	}
}
int GamePacman::getDirNow() {
	return dir_now;
}