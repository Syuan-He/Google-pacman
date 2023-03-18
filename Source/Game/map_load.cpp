#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>

using namespace game_framework;

//load map by .txt file which locate at dir(str)
void CGameStateRun::map_loader(string str) {
	gameMap = new int*[map_len[0]];
	for (int i = 0; i < map_len[0]; i++) {
		gameMap[i] = new int[map_len[1]];
	}

	std::ifstream in;
	in.open(str);
	int value, x = 0, y = 0;
	while (in >> value) {
		gameMap[x][y] = value;
		y++;
		if (y == map_len[1]) {
			y = 0;
			x++;
		}
	}
	in.close();
}
