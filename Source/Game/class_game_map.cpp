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
#include <array> 

using namespace game_framework;

//加載地圖
void GameMap::map_loader(string str) {
	string map_matrix = str + "/google_map.txt";
	string map_portal = str + "/portal_pos.txt";

	//載入地圖陣列
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

	//載入傳送門位置
	inputFile.open(map_portal, ifstream::in);
	while (getline(inputFile, line))
	{
		istringstream iss(line);
		array<int, 4> lineNumbers;
		for (int i = 0; i < 4; i++) {
			iss >> lineNumbers[i];
		}
		portal_position.push_back(lineNumbers);
	}
	inputFile.close();

	//載入背景圖檔
	Background.LoadBitmapByString({
		str + "/image/googleMap0.bmp",
		str + "/image/googleMap1.bmp",
		});
}

//偵測傳送門
pair<int,int> GameMap::portal_detect(int x, int y) {

	for (auto i : portal_position) {
		if (x == i[0] && y == i[1]) {
			return pair<int, int>(i[2], i[3]);
		}
		else if (x == i[2] && y == i[3]) {
			return pair<int, int>(i[0], i[1]);
		}
	}
	return pair<int, int>(-1, -1);
}

//回傳地圖陣列值
const vector<int>& GameMap::operator[](int i) const {
	return gameMap[i];
}