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

//加載地圖
void GameMap::map_loader(string str) {
	string map_matrix = str + "/google_map.txt";
	string map_portal = str + "/portal_pos.txt";

	ifstream inputFile;
	inputFile.open(map_matrix, ifstream::in);
	string line;
	int x = 0, y = 0;
	while (getline(inputFile, line))
	{
		istringstream token(line);
		string num;
		vector<int> Line;
		x = 0;
		while (token >> num)
		{
			Line.push_back(stoi(num));
			x++;
		}
		gameMap.push_back(Line);
		y++;
	}
	inputFile.close();

	//更新地圖大小
	map_len.set_value(y, x);
	y = 0;
	
	inputFile.open(map_portal, ifstream::in);
	while (getline(inputFile, line))
	{
		istringstream token(line);
		x = 0;
		string num;
		while (token >> num)
		{
			portal_position[y][x] = stoi(num);
			x++;
		}
		y++;
	}
	inputFile.close();

	Background.LoadBitmapByString({
		str + "/image/googleMap0.bmp",
		str + "/image/googleMap1.bmp",
		});
}

//偵測傳送門
int* GameMap::portal_detect(int x, int y) {
	if (x == portal_position[0][0] && y == portal_position[0][1]) {
		return portal_position[1];
	}
	else if (x == portal_position[1][0] && y == portal_position[1][1]) {
		return portal_position[0];
	}
	else {
		return nullptr;
	}
}

//回傳地圖陣列值
const vector<int>& GameMap::operator[](int i) const {
	return gameMap[i];
}